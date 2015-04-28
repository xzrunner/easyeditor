#include "Snapshoot.h"

#include "dataset/ISymbol.h"
#include "dataset/ImageSaver.h"
#include "render/SpriteDraw.h"

#include <stdlib.h>
#include <string.h>
#include <gl/glew.h>

#include <easyimage.h>

namespace d2d
{

static const int DEFAULT_WIDTH = 800;
static const int DEFAULT_HEIGHT = 600;

Snapshoot::Snapshoot()
	: m_tex(0)
	, m_fbo(0)
{
	m_width = DEFAULT_WIDTH;
	m_height = DEFAULT_HEIGHT;

	createFBO();

#ifdef TEST_RESAMPLING
	m_old_shader = new d2d::SpriteShader;
	m_old_shader->Load();
	m_new_shader = new eimage::LanczosResamplingShader;
	m_new_shader->Load();
#endif
}

Snapshoot::Snapshoot(int width, int height)
	: m_tex(0)
	, m_fbo(0)
{
	m_width = width;
	m_height = height;

	createFBO();
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

	ReadPixels(pixels, w, h);

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

void Snapshoot::DrawSprite(const ISprite* sprite, bool clear) const
{
	drawFBO(sprite, clear, m_width, m_height);
}

void Snapshoot::DrawSprite(const ISprite* sprite, bool clear, int width, int height) const
{
	drawFBO(sprite, clear, width, height);
}

void Snapshoot::SaveToFile(const std::string& filename) const
{
	SaveToFile(filename, m_width, m_height);
}

void Snapshoot::SaveToFile(const std::string& filename, int width, int height) const
{
	size_t size = width * height * 4;
	unsigned char* pixels = new unsigned char[size];
	if(!pixels) return;
	memset(&pixels[0], 0, size);	

	ReadPixels(pixels, width, height);

	ImageSaver::storeToFile(pixels, width, height, filename, ImageSaver::e_png);

	delete[] pixels;
}

void Snapshoot::createFBO()
{
	//initOpenGLExtensions();

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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_width, (GLsizei)m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	unsigned char* empty_data = new unsigned char[m_width*m_height*4];
	if(!empty_data) return;
	memset(empty_data, 0, m_width*m_height*4);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)m_width, (GLsizei)m_height, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
	delete[] empty_data;

	glGenFramebuffersEXT(1, &m_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
	assert(checkFramebufferStatus());

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderMgr::Instance()->GetFboID());
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
		glDeleteFramebuffersEXT(1, &m_fbo);
		m_fbo = 0;
	}
}

void Snapshoot::drawFBO(const ISymbol* symbol, bool whitebg, float scale) const
{
	ShaderMgr* shader = ShaderMgr::Instance();
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

	Matrix mt;
	float dx = symbol->getSize().xCenter();
	float dy = symbol->getSize().yCenter();
	mt.translate(dx, dy);
	SpriteDraw::drawSprite(symbol, mt, d2d::Vector(0, 0), 0.0f, scale, -scale);

	// set fbo to force flush
	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
 	shader->SetFBO(0);
 	shader->SetTexture(0);
}

void Snapshoot::drawFBO(const ISprite* sprite, bool clear, int width, int height) const
{
	ShaderMgr* shader = ShaderMgr::Instance();
#ifdef TEST_RESAMPLING
 	shader->null();
 	shader->SetSpriteShader(m_new_shader, false);
#endif
	shader->SetFBO(m_fbo);
	shader->sprite();

	if (clear) {
	 	glClearColor(0, 0, 0, 0);
	 	glClear(GL_COLOR_BUFFER_BIT);
	}

	shader->SetModelView(Vector(0, 0), 1);
	shader->SetProjection(width, height);
	glViewport(0, 0, width, height);

// 	Rect r = sprite->getSymbol().getSize();
// 	mt.translate(-r.xCenter(), r.yCenter());

	Matrix mt;
	mt.setScale(1, -1);
	SpriteDraw::drawSprite(sprite, mt);

	shader->SetFBO(0);
	shader->SetTexture(0);

#ifdef TEST_RESAMPLING
	d2d::ShaderMgr::Instance()->SetSpriteShader(m_old_shader, false);
#endif
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

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
		std::cout << "++ fbo: [ERROR] GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE. \n";
		return 0;

	default:
		std::cout << "++ fbo: [ERROR] Unknow error." << std::endl;
		return 0;
	}	
}

void Snapshoot::ReadPixels(unsigned char* pixels, int width, int height) const
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderMgr::Instance()->GetFboID());
}

}