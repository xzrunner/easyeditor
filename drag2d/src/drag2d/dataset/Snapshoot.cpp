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

unsigned char* Snapshoot::outputToMemory(const ISymbol* symbol, bool whitebg,
										 float scale) const
{
	drawFBO(symbol, whitebg, scale);

	int w = symbol->getSize().xLength() * scale,
		h = symbol->getSize().yLength() * scale;
	size_t size = w*h*4;
	unsigned char* pixels = new unsigned char[size];
	if(!pixels) return NULL;
	memset(&pixels[0], 0, size);	

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderNew::Instance()->GetFboID());

	return pixels;
}

void Snapshoot::outputToImageFile(const ISymbol* symbol, const std::string& filename,
								  float scale) const
{
	int w = symbol->getSize().xLength() * scale,
		h = symbol->getSize().yLength() * scale;
	unsigned char* pixels = outputToMemory(symbol, false, scale);
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
	glGenFramebuffersEXT(1, (GLuint*)&m_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);

	int status = checkFramebufferStatus();
	assert(status);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderNew::Instance()->GetFboID());
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

void Snapshoot::drawFBO(const ISymbol* symbol, bool whitebg, float scale) const
{
	ShaderNew* shader = ShaderNew::Instance();
	shader->SetFBO(m_fbo);
	shader->sprite();

 	if (whitebg) {
 		glClearColor(1, 1, 1, 1);
 	} else {
		glClearColor(0, 0, 0, 0);
	}	
	glClear(GL_COLOR_BUFFER_BIT);

	d2d::Rect rect = symbol->getSize();
 	int w = rect.xLength() * scale,
 		h = rect.yLength() * scale;
	shader->SetModelView(Vector(0, 0), 1);
	shader->SetProjection(w, h);
	glViewport(0, 0, w, h);

	Screen scr(w, h);
	scr.SetCamera(Vector(-rect.xCenter(), -rect.yCenter()) * scale, Vector(1, -1));

	SpriteDraw::drawSprite(scr, symbol, Matrix(), d2d::Vector(0, 0), 0.0f, scale, -scale);

	// set fbo to force flush
	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
 	shader->SetFBO(0);
 	shader->SetTexture(0);
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