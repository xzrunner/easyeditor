#include <gl/glew.h>

#include "IStageCanvas.h"

#include "common/Exception.h"
#include "view/ExceptionDlg.h"
#include "view/EditPanelImpl.h"
#include "view/KeysState.h"
#include "render/ShaderMgr.h"
#include "render/ShaderContext.h"
#include "render/RenderContext.h"
#include "render/RenderContextStack.h"
#include "render/GL.h"
#include "message/SetCanvasDirtySJ.h"
#include "message/subject_id.h"

namespace d2d
{

BEGIN_EVENT_TABLE(IStageCanvas, wxGLCanvas)
	EVT_SIZE(IStageCanvas::OnSize)
	EVT_PAINT(IStageCanvas::OnPaint)
//	EVT_ERASE_BACKGROUND(IStageCanvas::OnEraseBackground)
 	EVT_MOUSE_EVENTS(IStageCanvas::OnMouse)
 	EVT_KEY_DOWN(IStageCanvas::OnKeyDown)
	EVT_KEY_UP(IStageCanvas::OnKeyUp)
	EVT_TIMER(TIMER_ID, IStageCanvas::OnTimer)
	EVT_KILL_FOCUS(IStageCanvas::OnKillFocus)
END_EVENT_TABLE()

static const int GL_ATTRIB[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
								  WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
								  WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

static const float FPS = 30;

IStageCanvas::IStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, wxGLContext* glctx)
	: wxGLCanvas(stage_wnd, wxID_ANY, GL_ATTRIB)
	, m_share_context(false)
	, m_stage(stage)
 	, m_camera(stage->GetCamera())
	, m_screen(stage->GetCamera())
 	, m_width(0), m_height(0)
	, m_dirty(false)
	, m_cam_dirty(false)
	, m_render_context(new RenderContext)
	, m_timer(this, TIMER_ID)
	, m_version(0)
{
	if (glctx) {
		m_share_context = true;
		m_gl_context = glctx;
	} else {
		m_share_context = false;
		m_gl_context = new wxGLContext(this);
		Init();
	}

	m_bg_color.set(0.5f, 0.5f, 0.5f, 1);
	m_timer.Start(1000 / FPS);

	RegistSubject(SetCanvasDirtySJ::Instance());

	RenderContextStack::Instance()->Push(this, m_render_context);
}

IStageCanvas::~IStageCanvas()
{
	m_timer.Stop();

	if (!m_share_context) {
		delete m_gl_context;
	}

	RenderContextStack::Instance()->Pop();
}

void IStageCanvas::SetBgColor(const Colorf& color)
{
	m_bg_color = color;
}

void IStageCanvas::SetCurrentCanvas()
{
	SetCurrent(*m_gl_context);

	RenderContextStack::Instance()->SetCurrCtx(m_render_context);

	Vector offset;
	float scale;
	if (m_render_context->GetModelView(offset, scale)) {
		m_render_context->SetModelView(offset, scale);
	}

	int width, height;
	if (m_render_context->GetProjection(width, height)) {
		m_render_context->SetProjection(width, height);
		GL::Viewport(0, 0, width, height);
	}
}

void IStageCanvas::OnNotify(int sj_id, void* ud) 
{
	switch (sj_id)
	{
	case MSG_SET_CANVAS_DIRTY:
		m_dirty = true;
		break;
	}
}

void IStageCanvas::Init()
{
	SetCurrentCanvas();

	// prepare 2d
	// todo: move to child, for defferent init (such as 3d ?)
	ShaderMgr::Instance();

	if (glewInit() != GLEW_OK) {
		exit(1);
	}

	try {
		ShaderContext::Reload();
		ShaderContext::Reset();

		glEnable(GL_TEXTURE_2D);
	} catch (Exception& e) {
		ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();	
	}
}

void IStageCanvas::OnSize(wxSizeEvent& event)
{
	SetCurrentCanvas();

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

	OnDrawWhole();
	m_dirty = false;
	m_cam_dirty = false;

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
	m_stage->OnMouse(event);

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

void IStageCanvas::OnTimer(wxTimerEvent& event)
{
	OnTimer();

	++m_version;
	bool dirty = m_stage->Update(m_version);
	if (dirty) {
		m_dirty = dirty;
	}

	if (m_dirty || m_cam_dirty) {
		Refresh();
	}
}

void IStageCanvas::OnKillFocus(wxFocusEvent& event)
{
	m_stage->GetKeyState().Reset();
}

}