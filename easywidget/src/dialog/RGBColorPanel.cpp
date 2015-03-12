#include "RGBColorPanel.h"

#include <gl/GLee.h>

namespace ewidget
{

RGBColorPanel::RGBColorPanel(egui::Window* parent, int w, int h)
	: egui::Panel(parent, w, h)
{
	m_canvas = new Canvas(this);
}

RGBColorPanel::~RGBColorPanel()
{
	delete m_canvas;
}

void RGBColorPanel::SetColor(const Colorf& begin, const Colorf& end)
{
	m_canvas->SetColor(begin, end);
}

void RGBColorPanel::OnSize(int w, int h)
{
	m_canvas->SetSize(w, h);
}

//////////////////////////////////////////////////////////////////////////
// class RGBColorPanel::Canvas
//////////////////////////////////////////////////////////////////////////

RGBColorPanel::Canvas::
Canvas(egui::Window* parent)
	: SimpleGLCanvas(parent)
{
}

void RGBColorPanel::Canvas::
SetColor(const Colorf& begin, const Colorf& end)
{
	m_col_begin = begin;
	m_col_end = end;
//	Refresh();
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