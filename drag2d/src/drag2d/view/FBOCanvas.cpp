#include <gl/glew.h>

#include "FBOCanvas.h"

#include "common/Exception.h"
#include "render/RenderContext.h"
#include "render/ShaderMgrBase.h"
#include "render/ShaderMgr.h"
#include "view/EditPanel.h"

namespace d2d
{

BEGIN_EVENT_TABLE(FBOCanvas, wxGLCanvas)
	EVT_SIZE(FBOCanvas::OnSize)
	EVT_PAINT(FBOCanvas::OnPaint)
	EVT_ERASE_BACKGROUND(FBOCanvas::onEraseBackground)
END_EVENT_TABLE()

static int gl_attrib[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
						WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
						WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

FBOCanvas::FBOCanvas(EditPanel* stage)
	: wxGLCanvas(stage, wxID_ANY, gl_attrib)
	, m_camera(stage->getCamera())
	, m_screen(stage->getCamera())
 	, m_inited(false)
 	, m_context(new wxGLContext(this))
	, m_tex(0)
	, m_fbo(0)
{
}

FBOCanvas::~FBOCanvas()
{
	delete m_context;
}

void FBOCanvas::SetCurrentCanvas()
{
	if (m_inited) {
		SetCurrent(*m_context);
	}
}

void FBOCanvas::InitGL()
{
	if (glewInit() != GLEW_OK) {
		exit(1);
	}

	ShaderMgr::Instance()->null();

	OnSize(wxSizeEvent(m_parent->GetSize()));

	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	if (RenderContext::SHADER_MGR) {
		RenderContext::SHADER_MGR->reload();
	}
}

void FBOCanvas::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	int w = size.GetWidth(),
		h = size.GetHeight();
	ReleaseFBO();
	CreateFBO(w, h);

	OnSize(w, h);
}

void FBOCanvas::OnPaint(wxPaintEvent& event)
{
	// Makes the OpenGL state that is represented by the OpenGL rendering 
	// context context current
	SetCurrent(*m_context);

	if (!m_inited) {
		InitGL();
		m_inited = true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Draw to FBO
	//////////////////////////////////////////////////////////////////////////

	glClearColor(0.8, 0.8, 0.8, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderMgr::Instance()->SetFBO(m_fbo);

	OnDraw();

	if (RenderContext::SHADER_MGR) {
		RenderContext::SHADER_MGR->Flush();
	}

	//////////////////////////////////////////////////////////////////////////
	// Draw to Screen
	//////////////////////////////////////////////////////////////////////////

	ShaderMgr::Instance()->SetFBO(0);

	d2d::Vector vertices[4];
	vertices[0].set(-400, -400);
	vertices[1].set(-400,  400);
	vertices[2].set( 400,  400);
	vertices[3].set( 400, -400);

	d2d::Vector texcoords[4];
	texcoords[0].set(0, 0);
	texcoords[1].set(0, 1);
	texcoords[2].set(1, 1);
	texcoords[3].set(1, 0);

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->sprite();
	shader->Draw(vertices, texcoords, m_tex);

	if (RenderContext::SHADER_MGR) {
		RenderContext::SHADER_MGR->Flush();
	}

	//////////////////////////////////////////////////////////////////////////

	glFlush();
	SwapBuffers();
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

void FBOCanvas::onEraseBackground(wxEraseEvent& event)
{	
}

}