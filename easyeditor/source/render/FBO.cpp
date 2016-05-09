#include "FBO.h"
#include "Symbol.h"
#include "Shape.h"
#include "Exception.h"
#include "SpriteRenderer.h"
#include "RenderContextStack.h"
#include "GL.h"
#include "EE_ShaderLab.h"

#include <shaderlab.h>

#include <gl/glew.h>

namespace ee
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

void FBO::DrawSprite(const Sprite* sprite, bool clear, float dx, float dy)
{
	DrawFBO(sprite, clear, m_width, m_height, dx, dy, 1);
}

void FBO::DrawSprite(const Sprite* sprite, bool clear, int width, int height, 
					 float dx, float dy, float scale)
{
	DrawFBO(sprite, clear, width, height, dx, dy, scale);
}

void FBO::DrawShape(const Shape* shape, bool clear, int width, int height)
{
	DrawFBO(shape, clear, width, height);
}

void FBO::DrawSymbol(const Symbol* symbol, bool whitebg, float scale)
{
	DrawFBO(symbol, whitebg, scale);
}

void FBO::ReadPixels(unsigned char* pixels, int width, int height) const
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sl::ShaderMgr::Instance()->GetContext()->GetTarget());
}

void FBO::CreateFBO(int w, int h)
{
	if (w == 0 || h == 0) {
		return;
	}

	unsigned char* empty_data = new unsigned char[w*h*4];
	if(!empty_data) return;
	memset(empty_data, 0x00, w*h*4);
	m_tex = ShaderLab::Instance()->CreateTexture(empty_data, w, h, EE_TEXTURE_RGBA8);
	int gl_id = ShaderLab::Instance()->GetTexGLID(m_tex);

	glGenFramebuffersEXT(1, &m_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
//	mgr->SetFBO(m_fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, gl_id, 0);

	std::string msg;
	int status = CheckFramebufferStatus(msg);
	if (!status) {
		throw Exception(msg.c_str());
	}

// 	mgr->SetTexture(0);
// 	mgr->SetFBO(0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, sl::ShaderMgr::Instance()->GetContext()->GetTarget());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::ReleaseFBO()
{
	ShaderLab::Instance()->ReleaseTexture(m_tex);

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

void FBO::DrawFBO(const Symbol* symbol, bool whitebg, float scale)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	glBindFramebufferEXT(GL_FRAMEBUFFER, m_fbo);
	mgr->GetContext()->SetTarget(m_fbo);
	mgr->SetShader(sl::SPRITE2);

	if (whitebg) {
		glClearColor(1, 1, 1, 1);
	} else {
		glClearColor(0, 0, 0, 0);
	}	
	glClear(GL_COLOR_BUFFER_BIT);

	RenderContextStack* ctx_stack = RenderContextStack::Instance();

	bool has_context = true;

	sm::vec2 last_offset;
	float last_scale;
	if (!ctx_stack->GetModelView(last_offset, last_scale)) {
		has_context = false;
	}

	int last_w, last_h;
	if (!ctx_stack->GetProjection(last_w, last_h)) {
		has_context = false;
	}

	Rect rect = symbol->GetSize();
	int w = static_cast<int>(rect.Width() * scale),
		h = static_cast<int>(rect.Height() * scale);
	ctx_stack->SetModelView(sm::vec2(0, 0), 1);
	ctx_stack->SetProjection(w, h);
	GL::Viewport(0, 0, w, h);

	sm::mat4 mt;
	float dx = -symbol->GetSize().CenterX();
	float dy = symbol->GetSize().CenterY();
	mt.Translate(dx * scale, dy * scale, 0);
	SpriteRenderer::Draw(symbol, mt, sm::vec2(0, 0), 0.0f, scale, -scale);

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	mgr->GetShader()->Commit();

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	mgr->GetContext()->SetTarget(0);
	mgr->GetContext()->SetTexture(0, 0);

	if (has_context) {
		ctx_stack->SetModelView(last_offset, last_scale);
		ctx_stack->SetProjection(last_w, last_h);
		GL::Viewport(0, 0, last_w, last_h);
	}
}

void FBO::DrawFBO(const Sprite* sprite, bool clear, int width, int height, 
				  float dx, float dy, float scale)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	glBindFramebufferEXT(GL_FRAMEBUFFER, m_fbo);
	mgr->GetContext()->SetTarget(m_fbo);
	mgr->SetShader(sl::SPRITE2);

	if (clear) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	RenderContextStack* ctx_stack = RenderContextStack::Instance();

	bool has_context = true;

	sm::vec2 last_offset;
	float last_scale;
	if (!ctx_stack->GetModelView(last_offset, last_scale)) {
		has_context = false;
	}

	int last_w, last_h;
	if (!ctx_stack->GetProjection(last_w, last_h)) {
		has_context = false;
	}

	ctx_stack->SetModelView(sm::vec2(0, 0), 1);
	ctx_stack->SetProjection(width, height);
	GL::Viewport(0, 0, width, height);

	sm::mat4 mt;
	mt.Scale(scale, -scale, 1);
	mt.Translate(-dx, -dy, 0);
	RenderParams params(mt);
	params.set_shader = false;
	SpriteRenderer::Draw(sprite, NULL, params);

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	mgr->GetShader()->Commit();

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	mgr->GetContext()->SetTarget(0);
	mgr->GetContext()->SetTexture(0, 0);

	if (has_context) {
		ctx_stack->SetModelView(last_offset, last_scale);
		ctx_stack->SetProjection(last_w, last_h);	
		GL::Viewport(0, 0, last_w, last_h);
	}
}

void FBO::DrawFBO(const Shape* shape, bool clear, int width, int height)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	if (clear) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	RenderContextStack* ctx_stack = RenderContextStack::Instance();

	bool has_context = true;

	sm::vec2 last_offset;
	float last_scale;
	if (!ctx_stack->GetModelView(last_offset, last_scale)) {
		has_context = false;
	}

	int last_w, last_h;
	if (!ctx_stack->GetProjection(last_w, last_h)) {
		has_context = false;
	}

	ctx_stack->SetModelView(sm::vec2(0, 0), 1);
	ctx_stack->SetProjection(width, height);
	GL::Viewport(0, 0, width, height);

	shape->Draw(sm::mat4::Scaled(1, -1, 1));

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	if (has_context) {
		ctx_stack->SetModelView(last_offset, last_scale);
		ctx_stack->SetProjection(last_w, last_h);	
	}
}

}