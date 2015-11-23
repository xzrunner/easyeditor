#define NOMINMAX
#include <gl/glew.h>

#include "EJScreen.h"

#define STRINGIFY(A)  #A
#include "SpriteShader.vert"
#include "SpriteShader.frag"

#include <ejoy2d.h>
#include <dtex.h>

struct ej_package;

namespace eejoy2d
{

// static const int TEX_W = 2048;
// static const int TEX_H = 2048;

static const int TEX_W = 480 * 8;
static const int TEX_H = 320 * 8;

EJScreen* EJScreen::m_instance = NULL;
bool EJScreen::m_loaded = false;

EJScreen* EJScreen::Instance()
{
	return m_loaded ? m_instance : NULL;
}

EJScreen* EJScreen::Create()
{
	if (!m_instance) {
		m_instance = new EJScreen();
		m_instance->Load();
	}

	m_loaded = true;

	return m_instance;
}

void EJScreen::Load()
{
	assert(!m_loaded);

	m_loaded = true;

	CreateRes();

//	dtex_shader_load();
}

void EJScreen::Unload()
{
	ReleaseRes();

//	dtex_shader_unload();
}

void EJScreen::Bind()
{
	glViewport(0, 0, TEX_W, TEX_H);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
//	dtex_shader_target(m_fbo);
}

void EJScreen::UnBind()
{
	dtex_shader_flush();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, d2d::ShaderMgr::Instance()->GetFboID());

//	dtex_shader_target(d2d::ShaderMgr::Instance()->GetFboID());

	float w, h, s;
	dtex_get_screen(&w, &h, &s);
	glViewport(0, 0, w, h);
}

int EJScreen::GetWidth() const
{
	return TEX_W;
}

int EJScreen::GetHeight() const
{
	return TEX_H;
}

GLuint EJScreen::GetTexID() const
{
	return m_tex;
}

void EJScreen::OnSize(int w, int h)
{
	dtex_set_screen(w, h, 1);
}

void EJScreen::Clear()
{
//	glClearColor(0, 0, 0, 0);
	glClearColor(0.5, 0.1, 0.1, 0.1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void EJScreen::DebugDraw() const
{
	dtexf_debug_draw();
	dtex_shader_flush();
}

void EJScreen::CreateRes()
{
	// init tex

	int tex_id = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, (GLuint*)&tex_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)TEX_W, (GLsizei)TEX_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

 	uint8_t* empty_data = new uint8_t[TEX_W*TEX_H*4];
 	memset(empty_data, 0xaa, TEX_W*TEX_H*4);
 	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)TEX_W, (GLsizei)TEX_H, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
 	delete[] empty_data;

	m_tex = tex_id;

	// init fbo

	int fbo_id = 0;
	glGenFramebuffersEXT(1, (GLuint*)&fbo_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_tex, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		throw d2d::Exception("Create FBO error: %d", status);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, d2d::ShaderMgr::Instance()->GetFboID());

	m_fbo = fbo_id;
}

void EJScreen::ReleaseRes()
{
	glDeleteTextures(1, &m_tex);
	m_tex = 0;

	glDeleteFramebuffersEXT(1, &m_fbo);
	m_fbo = 0;
}

}