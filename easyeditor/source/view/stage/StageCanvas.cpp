#include <gl/glew.h>

#include "StageCanvas.h"
#include "Exception.h"
#include "ExceptionDlg.h"
#include "EditPanelImpl.h"
#include "KeysState.h"
#include "RenderContext.h"
#include "RenderContextStack.h"
#include "subject_id.h"
#include "GL.h"
#include "panel_msg.h"
#include "EE_ShaderLab.h"

#include <shaderlab.h>

namespace ee
{

BEGIN_EVENT_TABLE(StageCanvas, wxGLCanvas)
	EVT_SIZE(StageCanvas::OnSize)
	EVT_PAINT(StageCanvas::OnPaint)
//	EVT_ERASE_BACKGROUND(StageCanvas::OnEraseBackground)
 	EVT_MOUSE_EVENTS(StageCanvas::OnMouse)
 	EVT_KEY_DOWN(StageCanvas::OnKeyDown)
	EVT_KEY_UP(StageCanvas::OnKeyUp)
	EVT_TIMER(TIMER_ID, StageCanvas::OnTimer)
	EVT_KILL_FOCUS(StageCanvas::OnKillFocus)
END_EVENT_TABLE()

static const int GL_ATTRIB[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
								  WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
								  WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

static const float FPS = 30;

StageCanvas::StageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
						   wxGLContext* glctx, bool use_context_stack)
	: wxGLCanvas(stage_wnd, wxID_ANY, GL_ATTRIB)
	, m_share_context(false)
	, m_use_context_stack(use_context_stack)
	, m_stage(stage)
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

	m_bg_color.Set(0.5f, 0.5f, 0.5f, 1);
	m_timer.Start(1000 / FPS);

	RegistSubject(SetCanvasDirtySJ::Instance());

	if (m_use_context_stack) {
		RenderContextStack::Instance()->Push(this, m_render_context);
	}
}

StageCanvas::~StageCanvas()
{
	m_timer.Stop();

	if (!m_share_context) {
		delete m_gl_context;
	}

	if (m_use_context_stack) {
		RenderContextStack::Instance()->Pop();
	}
}

void StageCanvas::SetBgColor(const Colorf& color)
{
	m_bg_color = color;
}

void StageCanvas::SetCurrentCanvas()
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

void StageCanvas::OnNotify(int sj_id, void* ud) 
{
	switch (sj_id)
	{
	case MSG_SET_CANVAS_DIRTY:
		m_dirty = true;
		break;
	}
}

void StageCanvas::Init()
{
	SetCurrentCanvas();

	if (glewInit() != GLEW_OK) {
		exit(1);
	}

	// prepare 2d
	// todo: move to child, for defferent init (such as 3d ?)
	ShaderLab::Instance()->Init();

	try {
// 		ShaderContext::Reload();
// 		ShaderContext::Reset();

		glEnable(GL_TEXTURE_2D);
	} catch (Exception& e) {
		ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();	
	}
}

void StageCanvas::OnSize(wxSizeEvent& event)
{
	SetCurrentCanvas();

	wxSize size = event.GetSize();
 	m_width = size.GetWidth();
 	m_height = size.GetHeight();
	OnSize(m_width, m_height);
}

void StageCanvas::OnPaint(wxPaintEvent& event)
{
	// Makes the OpenGL state that is represented by the OpenGL rendering 
	// context context current
	SetCurrentCanvas();

	OnDrawWhole();
	m_dirty = false;
	m_cam_dirty = false;

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	glFlush();
	SwapBuffers();

//	wxPaintDC dc(this);
//	OnDrawDC();
}

// void StageCanvas::OnEraseBackground(wxEraseEvent& event)
// {	
// }

void StageCanvas::OnMouse(wxMouseEvent& event)
{
	m_stage->OnMouse(event);

	// The handler of this event should normally call event.Skip() to allow the default processing 
	// to take place as otherwise the window under mouse wouldn't get the focus.
	if (event.LeftDown()) {
		event.Skip();
	}
}

void StageCanvas::OnKeyDown(wxKeyEvent& event)
{
	m_stage->OnKeyDown(event);
}

void StageCanvas::OnKeyUp(wxKeyEvent& event)
{
	m_stage->OnKeyUp(event);
}

void StageCanvas::OnTimer(wxTimerEvent& event)
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

void StageCanvas::OnKillFocus(wxFocusEvent& event)
{
	m_stage->GetKeyState().Reset();
}

}