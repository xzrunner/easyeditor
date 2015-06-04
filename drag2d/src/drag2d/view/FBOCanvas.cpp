#include <gl/glew.h>

#include "FBOCanvas.h"

#include "common/Exception.h"
#include "render/ShaderMgr.h"
#include "render/RenderContext.h"
#include "render/BlendShader.h"

namespace d2d
{

FBOCanvas::FBOCanvas(EditPanel* stage)
	: IStageCanvas(stage)
	, m_tex(0)
	, m_fbo(0)
{
}

FBOCanvas::~FBOCanvas()
{
	ReleaseFBO();
}

void FBOCanvas::OnSize(int w, int h)
{
	ReleaseFBO();
	CreateFBO(w, h);
}

void FBOCanvas::OnDrawWhole() const
{
	ShaderMgr* mgr = ShaderMgr::Instance();

//	glClearColor(0.8f, 0.8f, 0.8f, 1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//////////////////////////////////////////////////////////////////////////
	// Draw to FBO
	//////////////////////////////////////////////////////////////////////////
	mgr->sprite();
	BlendShader* shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
 	shader->SetBaseTexID(m_tex);
 	shader->SetBlendMode("exclusion");
	mgr->SetFBO(m_fbo);

	glClearColor(0.8f, 0.8f, 0.8f, 1);
//	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OnDrawSprites();

	//////////////////////////////////////////////////////////////////////////
	// Draw to Screen
	//////////////////////////////////////////////////////////////////////////

	mgr->SetFBO(0);
	mgr->SetTexture(0);

	mgr->Screen();
	mgr->DrawScreen(m_tex);
}

void FBOCanvas::CreateFBO(int w, int h)
{
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	unsigned char* empty_data = new unsigned char[w*h*4];
	if(!empty_data) return;
	memset(empty_data, 0, w*h*4);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)w, (GLsizei)h, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
	delete[] empty_data;

	glGenFramebuffersEXT(1, &m_fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		throw Exception("Create FBO error: %d", status);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderMgr::Instance()->GetFboID());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FBOCanvas::ReleaseFBO()
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

}