#ifdef EGUI_WX

#include "interface/GLCanvas.h"
#include "interface/Window.h"

#include <wx/glcanvas.h>
#include <wx/dcclient.h>

namespace egui
{

//////////////////////////////////////////////////////////////////////////
// class GLCanvasImpl
//////////////////////////////////////////////////////////////////////////

class GLCanvasImpl : public wxGLCanvas
{
public:
	GLCanvasImpl(wxWindow* parent, GLCanvas* canvas);
	virtual ~GLCanvasImpl();

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

private:
	GLCanvas* m_canvas;

	wxGLContext* m_context;

	DECLARE_EVENT_TABLE()

}; // GLCanvasImpl

BEGIN_EVENT_TABLE(GLCanvasImpl, wxGLCanvas)
	EVT_SIZE(GLCanvasImpl::OnSize)
	EVT_PAINT(GLCanvasImpl::OnPaint)
END_EVENT_TABLE()

static int gl_attrib[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1, WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1, WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

GLCanvasImpl::GLCanvasImpl(wxWindow* parent, GLCanvas* canvas)
	: wxGLCanvas(parent, wxID_ANY, NULL)
	, m_canvas(canvas)
	, m_context(new wxGLContext(this))
{
}

GLCanvasImpl::~GLCanvasImpl()
{
	delete m_context;
}

void GLCanvasImpl::OnSize(wxSizeEvent& event)
{
// 	wxSize size = event.GetSize();
// 	int w = size.x, h = size.y;
// 	
// 	glViewport(0, 0, w, h);
// 
// 	e2d::ShaderMgr* shader = e2d::ShaderMgr::Instance();
//  	shader->SetModelView(vec2(0, 0), 1);
//  	shader->SetProjection(w, h);

 	wxSize size = event.GetSize();
	m_canvas->OnSize(size.x, size.y);
}

void GLCanvasImpl::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	SetCurrent(*m_context);

	glClearColor(1, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_canvas->OnDraw();

	glFlush();
	SwapBuffers();
}


//////////////////////////////////////////////////////////////////////////
// class GLCanvas
//////////////////////////////////////////////////////////////////////////

GLCanvas::GLCanvas(Window* parent)
{
	wxWindow* raw_wnd = static_cast<wxWindow*>(parent->GetRawPtr());
	m_impl = new GLCanvasImpl(raw_wnd, this);
}

GLCanvas::~GLCanvas()
{
	delete m_impl;
}

void GLCanvas::SetSize(int w, int h)
{
	m_impl->SetSize(w, h);
}

}

#endif // EGUI_WX