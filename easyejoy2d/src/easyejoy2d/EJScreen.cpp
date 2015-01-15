#define NOMINMAX
#include <gl/GLee.h>

#include "EJScreen.h"

#include <shader.h>
#include <ejoy2d.h>

struct ej_package;

namespace eejoy2d
{

static const int WIDTH = 800;
static const int HEIGHT = 600;

EJScreen* EJScreen::m_instance = NULL;

static ej_package* PKG = NULL; 

EJScreen* EJScreen::Instance()
{
	if (!m_instance) {
		m_instance = new EJScreen();
	}
	return m_instance;
}

EJScreen::EJScreen()
{
	ej_screen info;
	info.w = 800;
	info.h = 600;
	info.scale = 1;
	ejoy_set_screen(&info);

	InitRes();
}

EJScreen::~EJScreen()
{
	glDeleteTextures(1, &m_tex);
	glDeleteFramebuffersEXT(1, &m_fbo);
}

void EJScreen::Load()
{
	shader_load();
}

void EJScreen::Unload()
{
	shader_unload();
}

void EJScreen::Bind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
}

void EJScreen::UnBind()
{
 	shader_flush();
 	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, d2d::ShaderMgr::Instance()->GetFboID());
}

void EJScreen::InitRes()
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)WIDTH, (GLsizei)HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

 	uint8_t* empty_data = new uint8_t[WIDTH*HEIGHT*4];
 	memset(empty_data, 0xaa, WIDTH*HEIGHT*4);
 	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
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

}