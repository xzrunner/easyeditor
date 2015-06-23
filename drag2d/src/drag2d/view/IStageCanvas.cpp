#include <gl/glew.h>

#include "IStageCanvas.h"

#include "common/Exception.h"
#include "view/ExceptionDlg.h"
#include "view/EditPanel.h"
#include "render/ShaderMgr.h"
#include "render/RenderContext.h"

namespace d2d
{

BEGIN_EVENT_TABLE(IStageCanvas, wxGLCanvas)
	EVT_SIZE(IStageCanvas::OnSize)
	EVT_PAINT(IStageCanvas::OnPaint)
//	EVT_ERASE_BACKGROUND(IStageCanvas::OnEraseBackground)
 	EVT_MOUSE_EVENTS(IStageCanvas::OnMouse)
 	EVT_KEY_DOWN(IStageCanvas::OnKeyDown)
	EVT_KEY_UP(IStageCanvas::OnKeyUp)
END_EVENT_TABLE()

static const int GL_ATTRIB[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
								  WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
								  WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

IStageCanvas::IStageCanvas(EditPanel* stage)
	: wxGLCanvas(stage, wxID_ANY, GL_ATTRIB)
	, m_stage(stage)
 	, m_camera(stage->getCamera())
	, m_screen(stage->getCamera())
 	, m_width(0), m_height(0)
 	, m_inited(false)
 	, m_context(new wxGLContext(this))
{
	m_bg_color.set(0.5f, 0.5f, 0.5f, 1);
}

IStageCanvas::~IStageCanvas()
{
	delete m_context;
}

void IStageCanvas::ResetInitState() 
{ 
	m_inited = false; 
}

void IStageCanvas::ResetViewport()
{
	OnSize(wxSizeEvent(m_parent->GetSize()));
}

void IStageCanvas::SetBgColor(const Colorf& color)
{
	m_bg_color = color;
}

void IStageCanvas::SetCurrentCanvas()
{
	SetCurrent(*m_context);
}

void IStageCanvas::InitGL()
{
	if (glewInit() != GLEW_OK) {
		exit(1);
	}

	try {
		ShaderMgr::Instance()->null();

		ResetViewport();

		glEnable(GL_TEXTURE_2D);

		if (RenderContext::SHADER_MGR) {
			RenderContext::SHADER_MGR->reload();
		}
	} catch (Exception& e) {
		ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();	
	}
}

void IStageCanvas::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
 	m_width = size.GetWidth();
 	m_height = size.GetHeight();
	OnSize(m_width, m_height);
}

void IStageCanvas::OnPaint(wxPaintEvent& event)
{
	// Makes the OpenGL state that is represented by the OpenGL rendering 
	// context context current
	SetCurrentCanvas();

	if (!m_inited)
	{
		InitGL();
		m_inited = true;
	}

	OnDrawWhole();

	glFlush();
	SwapBuffers();

//	wxPaintDC dc(this);
//	OnDrawDC();
}

// void IStageCanvas::OnEraseBackground(wxEraseEvent& event)
// {	
// }

void IStageCanvas::OnMouse(wxMouseEvent& event)
{
	m_stage->onMouse(event);

	// The handler of this event should normally call event.Skip() to allow the default processing 
	// to take place as otherwise the window under mouse wouldn't get the focus.
	if (event.LeftDown()) {
		event.Skip();
	}
}

void IStageCanvas::OnKeyDown(wxKeyEvent& event)
{
	m_stage->OnKeyDown(event);
}

void IStageCanvas::OnKeyUp(wxKeyEvent& event)
{
	m_stage->OnKeyUp(event);
}

}