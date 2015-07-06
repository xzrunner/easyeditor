#include "FBO.h"
#include "ShaderMgr.h"

#include "dataset/ISymbol.h"
#include "dataset/IShape.h"
#include "common/Exception.h"
#include "common/Matrix.h"
#include "render/SpriteRenderer.h"

#include <gl/glew.h>

namespace d2d
{

FBO::FBO()
	: m_width(0)
	, m_height(0)
	, m_tex(0)
	, m_fbo(0)
{
}

FBO::FBO(int width, int height)
	: m_width(width)
	, m_height(height)
	, m_tex(0)
	, m_fbo(0)
{
	CreateFBO(width, height);
}

FBO::~FBO()
{
	ReleaseFBO();
}

void FBO::ChangeSize(int width, int height)
{
	m_width = width;
	m_height = height;
	ReleaseFBO();
	CreateFBO(width, height);
}

void FBO::DrawSprite(const ISprite* sprite, bool clear, float dx, float dy)
{
	DrawFBO(sprite, clear, m_width, m_height, dx, dy);
}

void FBO::DrawSprite(const ISprite* sprite, bool clear, int width, int height, 
					 float dx, float dy)
{
	DrawFBO(sprite, clear, width, height, dx, dy);
}

void FBO::DrawShape(const IShape* shape, bool clear, int width, int height)
{
	DrawFBO(shape, clear, width, height);
}

void FBO::DrawSymbol(const ISymbol* symbol, bool whitebg, float scale)
{
	DrawFBO(symbol, whitebg, scale);
}

void FBO::ReadPixels(unsigned char* pixels, int width, int height) const
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderMgr::Instance()->GetFboID());
}

void FBO::CreateFBO(int w, int h)
{
	if (w == 0 || h == 0) {
		return;
	}

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->sprite();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	GLuint tex = 0;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, tex);
	mgr->SetTexture(tex);
	m_tex = tex;

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	unsigned char* empty_data = new unsigned char[w*h*4];
	if(!empty_data) return;
	memset(empty_data, 0xaa, w*h*4);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)w, (GLsizei)h, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
	delete[] empty_data;

	glGenFramebuffersEXT(1, &m_fbo);
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	mgr->SetFBO(m_fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);

	std::string msg;
	int status = CheckFramebufferStatus(msg);
	if (!status) {
		throw Exception(msg.c_str());
	}

	mgr->SetTexture(0);
	mgr->SetFBO(0);

// 	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mgr->GetFboID());
// 	glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::ReleaseFBO()
{
	if (m_tex != 0) {
		glDeleteTextures(1, (GLuint*)(&m_tex));
		m_tex = 0;
	}

	if (m_fbo != 0) {
		glDeleteFramebuffersEXT(1, &m_fbo);
		m_fbo = 0;
	}
}

int FBO::CheckFramebufferStatus(std::string& msg) const
{
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		msg = "FBO: Framebuffer complete.";
		return 1;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		msg = "FBO: [ERROR] Framebuffer incomplete: Attachment is NOT complete.";
		return 0;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		msg = "FBO: [ERROR] Framebuffer incomplete: No image is attached to FBO.";
		return 0;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		msg = "FBO: [ERROR] Framebuffer incomplete: Draw Buffer.";
		return 0;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		msg = "FBO: [ERROR] Framebuffer incomplete: Read Buffer.";
		return 0;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		msg = "FBO: [ERROR] Unsupported by FBO implementation.";
		return 0;

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
		msg = "FBO: [ERROR] GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE.";
		return 0;

	default:
		msg = "FBO: [ERROR] Unknow error.";
		return 0;
	}	
}

void FBO::DrawFBO(const ISymbol* symbol, bool whitebg, float scale)
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

	d2d::Rect rect = symbol->GetSize();
	int w = rect.xLength() * scale,
		h = rect.yLength() * scale;
	shader->SetModelView(Vector(0, 0), 1);
	shader->SetProjection(w, h);
	glViewport(0, 0, w, h);

	Matrix mt;
	float dx = -symbol->GetSize().xCenter();
	float dy = symbol->GetSize().yCenter();
	mt.translate(dx, dy);
	SpriteRenderer::Instance()->Draw(symbol, mt, d2d::Vector(0, 0), 0.0f, scale, -scale);

	// set fbo to force flush
	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	shader->SetFBO(0);
	shader->SetTexture(0);
}

void FBO::DrawFBO(const ISprite* sprite, bool clear, int width, int height, float dx, float dy)
{
	ShaderMgr* shader = ShaderMgr::Instance();
	shader->SetFBO(m_fbo);
	shader->sprite();

	if (clear) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	shader->SetModelView(Vector(0, 0), 1);
	shader->SetProjection(width, height);
	glViewport(0, 0, width, height);

	Matrix mt;
	mt.setScale(1, -1);
	mt.translate(-dx, -dy);
	SpriteRenderer::Instance()->Draw(sprite, mt, Colorf(1,1,1,1), Colorf(0,0,0,0), 
		Colorf(1, 0, 0, 0), Colorf(0, 1, 0, 0), Colorf(0, 0, 1, 0), false);

	// set fbo to force flush
	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	shader->SetFBO(0);
	shader->SetTexture(0);
}

void FBO::DrawFBO(const IShape* shape, bool clear, int width, int height)
{
	ShaderMgr* shader = ShaderMgr::Instance();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	if (clear) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	shader->SetModelView(Vector(0, 0), 1);
	shader->SetProjection(width, height);
	glViewport(0, 0, width, height);

	Matrix mt;
	mt.setScale(1, -1);
	shape->draw(mt);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

}