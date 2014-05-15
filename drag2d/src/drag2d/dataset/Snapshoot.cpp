#include "Snapshoot.h"

#include "dataset/ISymbol.h"
#include "dataset/ImageSaver.h"
#include "render/SpriteDraw.h"

#include <stdlib.h>
#include <string.h>

#include <opengl/opengl.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <easyanim.h>

namespace d2d
{

Snapshoot::Snapshoot()
	: m_tex(0)
	, m_fbo(0)
{
	createFBO(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

Snapshoot::Snapshoot(int width, int height)
	: m_tex(0)
	, m_fbo(0)
{
	createFBO(width, height);
}

Snapshoot::~Snapshoot()
{
	releaseFBO();
}

unsigned char* Snapshoot::outputToMemory(const ISymbol* symbol, bool whitebg) const
{
	drawFBO(symbol, whitebg);

	int w = symbol->getSize().xLength(),
		h = symbol->getSize().yLength();
	size_t size = w*h*4;
	unsigned char* pixels = new unsigned char[size];
	if(!pixels) return NULL;
	memset(&pixels[0], 0, size);	

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return pixels;
}

unsigned char* Snapshoot::outputAnimToMemory(const ISymbol* symbol, int index, bool whitebg) const
{
	const anim::Symbol* anim = dynamic_cast<const anim::Symbol*>(symbol);
	if (!anim) {
		return NULL;
	}

	drawAnimFBO(anim, index, whitebg);

	int w = symbol->getSize().xLength(),
		h = symbol->getSize().yLength();
	size_t size = w*h*4;
	unsigned char* pixels = new unsigned char[size];
	if(!pixels) return NULL;
	memset(&pixels[0], 0, size);	

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return pixels;
}

void Snapshoot::outputToImageFile(const ISymbol* symbol, const std::string& filename) const
{
	int w = symbol->getSize().xLength(),
		h = symbol->getSize().yLength();
	unsigned char* pixels = outputToMemory(symbol);
	ImageSaver::storeToFile(pixels, w, h, filename, ImageSaver::e_png);
	delete[] pixels;
}

void Snapshoot::createFBO(int width, int height)
{
	initOpenGLExtensions();

	// init texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	m_tex = tex;

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	unsigned char* empty_data = new unsigned char[width*height*4];
	if(!empty_data) return;
	memset(empty_data, 0, width*height*4);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)width, (GLsizei)height, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
	delete[] empty_data;

	// init fbo
	GLuint fbo;

	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	m_fbo = fbo;

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);

	int status = checkFramebufferStatus();
	assert(status);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Snapshoot::releaseFBO()
{
	if (m_tex != 0)
	{
		glDeleteTextures(1, (GLuint*)(&m_tex));
		m_tex = 0;
	}
	if (m_fbo != 0)
	{
		glDeleteFramebuffersEXT(1, (GLuint*)(&m_fbo));
		m_fbo = 0;
	}
}

void Snapshoot::drawFBO(const ISymbol* symbol, bool whitebg) const
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	if (whitebg) {
		glClearColor(1, 1, 1, 1);
	} else {
		glClearColor(0, 0, 0, 0);
	}	
	glClear(GL_COLOR_BUFFER_BIT);

	d2d::Rect rect = symbol->getSize();
	int w = rect.xLength(),
		h = rect.yLength();
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(
		rect.xMin,
		rect.xMax,
		rect.yMax,
		rect.yMin,
		0,
		1
		);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	SpriteDraw::drawSprite(symbol, d2d::Vector(0, 0));
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Snapshoot::drawAnimFBO(const anim::Symbol* symbol, int index, bool whitebg /*= false*/) const
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	if (whitebg) {
		glClearColor(1, 1, 1, 1);
	} else {
		glClearColor(0, 0, 0, 0);
	}	
	glClear(GL_COLOR_BUFFER_BIT);

	d2d::Rect rect = symbol->getSize();
	int w = rect.xLength(),
		h = rect.yLength();
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(
		rect.xMin,
		rect.xMax,
		rect.yMax,
		rect.yMin,
		0,
		1
		);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	anim::Tools::drawAnimSymbol(symbol, index);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

int Snapshoot::checkFramebufferStatus() const
{
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		std::cout << "++ fbo: Framebuffer complete." << std::endl;
		return 1;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		std::cout << "++ fbo: [ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
		return 0;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		std::cout << "++ fbo: [ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
		return 0;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		std::cout << "++ fbo: [ERROR] Framebuffer incomplete: Draw Buffer." << std::endl;
		return 0;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		std::cout << "++ fbo: [ERROR] Framebuffer incomplete: Read Buffer." << std::endl;
		return 0;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		std::cout << "++ fbo: [ERROR] Unsupported by FBO implementation." << std::endl;
		return 0;

	default:
		std::cout << "++ fbo: [ERROR] Unknow error." << std::endl;
		return 0;
	}	
}

}