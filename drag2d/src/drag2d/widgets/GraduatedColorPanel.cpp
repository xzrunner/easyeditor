#include "GraduatedColorPanel.h"

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

GraduatedColorPanel::Canvas::
Canvas(wxWindow* parent)
	: SimpleGLCanvas(parent)
{
}

void GraduatedColorPanel::Canvas::
SetColor(const d2d::Colorf& begin, const d2d::Colorf& end)
{
	m_col_begin = begin;
	m_col_end = end;
	Refresh();
}

void GraduatedColorPanel::Canvas::
OnDraw() const
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