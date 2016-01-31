#include "SimpleGLCanvas.h"
#include "ShaderMgr.h"

#include <wx/dcclient.h>

namespace ee
{

BEGIN_EVENT_TABLE(SimpleGLCanvas, wxGLCanvas)
	EVT_SIZE(SimpleGLCanvas::OnSize)
	EVT_PAINT(SimpleGLCanvas::OnPaint)
END_EVENT_TABLE()

static int gl_attrib[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1, WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1, WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

SimpleGLCanvas::SimpleGLCanvas(wxWindow* parent)
	: wxGLCanvas(parent, wxID_ANY, NULL)
	, m_context(new wxGLContext(this))
{
}

SimpleGLCanvas::~SimpleGLCanvas()
{
	delete m_context;
}

void SimpleGLCanvas::OnSize(wxSizeEvent& event)
{
// 	wxSize size = event.GetSize();
// 	int w = size.x, h = size.y;
// 	
// 	glViewport(0, 0, w, h);
// 
// 	ShaderMgr* shader = ShaderMgr::Instance();
// 	shader->SetModelView(Vector(0, 0), 1);
// 	shader->SetProjection(w, h);
}

void SimpleGLCanvas::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	SetCurrent(*m_context);

	glClearColor(1, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	OnDraw();

	glFlush();
	SwapBuffers();
}

// void SimpleGLCanvas::DrawColor() const
// {
// 	glBegin(GL_QUADS);
// 		glColor3f(m_col_begin.r, m_col_begin.g, m_col_begin.b);
// 		glVertex2f(-1, -1);
// 		glVertex2f(-1, 1);
// 		glColor3f(m_col_end.r, m_col_end.g, m_col_end.b);
// 		glVertex2f(1, 1);
// 		glVertex2f(1, -1);		
// 	glEnd();
// }

}