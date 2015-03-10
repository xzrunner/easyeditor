#include "GraduatedColorPanel.h"

#include <wx/dcbuffer.h>

namespace d2d
{

BEGIN_EVENT_TABLE(GraduatedColorPanel, wxPanel)
	EVT_SIZE(GraduatedColorPanel::OnSize)
END_EVENT_TABLE()

GraduatedColorPanel::GraduatedColorPanel(wxWindow* parent, wxSize size)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, size, wxBORDER_DEFAULT)
{
	m_canvas = new Canvas(this);
}

GraduatedColorPanel::~GraduatedColorPanel()
{
	delete m_canvas;
}

void GraduatedColorPanel::SetColor(const d2d::Colorf& begin, const d2d::Colorf& end)
{
	m_canvas->SetColor(begin, end);
}

void GraduatedColorPanel::OnSize(wxSizeEvent& event)
{
	m_canvas->SetSize(event.GetSize());
}

//////////////////////////////////////////////////////////////////////////
// class GraduatedColorPanel::Canvas
//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(GraduatedColorPanel::Canvas, wxGLCanvas)
	EVT_SIZE(GraduatedColorPanel::Canvas::OnSize)
	EVT_PAINT(GraduatedColorPanel::Canvas::OnPaint)
END_EVENT_TABLE()

static int gl_attrib[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1, WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1, WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

GraduatedColorPanel::Canvas::
Canvas(wxWindow* parent)
	: wxGLCanvas(parent, wxID_ANY, NULL)
	, m_context(new wxGLContext(this))
{
}

GraduatedColorPanel::Canvas::
~Canvas()
{
	delete m_context;
}

void GraduatedColorPanel::Canvas::
SetColor(const d2d::Colorf& begin, const d2d::Colorf& end)
{
	m_col_begin = begin;
	m_col_end = end;
	Refresh();
}

void GraduatedColorPanel::Canvas::
OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	int w = size.x, h = size.y;
	
	glViewport(0, 0, w, h);

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->SetModelView(d2d::Vector(0, 0), 1);
	shader->SetProjection(w, h);
}

void GraduatedColorPanel::Canvas::
OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	SetCurrent(*m_context);

// 	bool inited = false;
// 	if (!inited) {
// 		OnSize(wxSizeEvent(m_parent->GetSize()));
// 		inited = true;
// 	}

	glClearColor(1, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawColor();

	glFlush();
	SwapBuffers();
}

void GraduatedColorPanel::Canvas::
DrawColor() const
{
	glBegin(GL_QUADS);
		glColor3f(m_col_begin.r, m_col_begin.g, m_col_begin.b);
		glVertex2f(-1, -1);
		glVertex2f(-1, 1);
		glColor3f(m_col_end.r, m_col_end.g, m_col_end.b);
		glVertex2f(1, 1);
		glVertex2f(1, -1);		
	glEnd();
}

}