#include "RGBColorPanel.h"

namespace ee
{

BEGIN_EVENT_TABLE(RGBColorPanel, wxPanel)
	EVT_SIZE(RGBColorPanel::OnSize)
END_EVENT_TABLE()

RGBColorPanel::RGBColorPanel(wxWindow* parent, wxSize size)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, size, wxBORDER_DEFAULT)
{
	m_canvas = std::make_shared<Canvas>(this);
}

void RGBColorPanel::SetColor(const pt2::Color& begin, const pt2::Color& end)
{
	m_canvas->SetColor(begin, end);
}

void RGBColorPanel::OnSize(wxSizeEvent& event)
{
	m_canvas->SetSize(event.GetSize());
}

//////////////////////////////////////////////////////////////////////////
// class RGBColorPanel::Canvas
//////////////////////////////////////////////////////////////////////////

RGBColorPanel::Canvas::
Canvas(wxWindow* parent)
	: SimpleGLCanvas(parent)
{
}

void RGBColorPanel::Canvas::
SetColor(const pt2::Color& begin, const pt2::Color& end)
{
	m_col_begin = begin;
	m_col_end = end;
	Refresh(true);
}

void RGBColorPanel::Canvas::
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