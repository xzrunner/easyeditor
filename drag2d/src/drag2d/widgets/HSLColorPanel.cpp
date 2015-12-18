#include "HSLColorPanel.h"

#include "common/color_convert.h"

namespace d2d
{

BEGIN_EVENT_TABLE(HSLColorPanel, wxPanel)
	EVT_SIZE(HSLColorPanel::OnSize)
END_EVENT_TABLE()

HSLColorPanel::HSLColorPanel(wxWindow* parent, wxSize size)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, size, wxBORDER_DEFAULT)
{
	m_canvas = new Canvas(this);
}

HSLColorPanel::~HSLColorPanel()
{
	delete m_canvas;
}

void HSLColorPanel::SetColor(const Colorf& begin, const Colorf& end)
{
	m_canvas->SetColor(begin, end);
}

void HSLColorPanel::OnSize(wxSizeEvent& event)
{
	m_canvas->SetSize(event.GetSize());
}

//////////////////////////////////////////////////////////////////////////
// class HSLColorPanel::Canvas
//////////////////////////////////////////////////////////////////////////

HSLColorPanel::Canvas::
Canvas(wxWindow* parent)
	: SimpleGLCanvas(parent)
{
}

void HSLColorPanel::Canvas::
SetColor(const Colorf& begin, const Colorf& end)
{
	m_col_begin = begin;
	m_col_end = end;
	Refresh(true);
}

void HSLColorPanel::Canvas::
OnDraw() const
{
	const int COUNT = 100;
	const Colorf& begin = m_col_begin, end = m_col_end;

	glBegin(GL_QUADS);
	for (int i = 0; i < COUNT; ++i)
	{
		float p = (float)i / COUNT;
		float h = (end.r - begin.r) * p + begin.r;
		float s = (end.g - begin.g) * p + begin.g;
		float l = (end.b - begin.b) * p + begin.b;

		Colorf rgb = hsl2rgb(h, s, l);
		glColor3f(rgb.r, rgb.g, rgb.b);

		float sx = -1 + 2.0f / COUNT * i,
			ex = -1 + 2.0f / COUNT * (i + 1);
		glVertex2f(sx, -1);
		glVertex2f(sx, 1);
		glVertex2f(ex, 1);
		glVertex2f(ex, -1);		
	}
	glEnd();
}

}