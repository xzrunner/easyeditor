#ifndef _DRAG2D_GRADUATED_COLOR_PANEL_H_
#define _DRAG2D_GRADUATED_COLOR_PANEL_H_

#include <drag2d.h>

namespace d2d
{

class GraduatedColorPanel : public wxPanel
{
public:
	GraduatedColorPanel(wxWindow* parent, wxSize size);
	virtual ~GraduatedColorPanel();

	void SetColor(const d2d::Colorf& begin, const d2d::Colorf& end);

private:
	void OnSize(wxSizeEvent& event);

private:
	class Canvas : public wxGLCanvas
	{
	public:
		Canvas(wxWindow* parent);
		virtual ~Canvas();

		void SetColor(const d2d::Colorf& begin, const d2d::Colorf& end);

	private:
		void OnSize(wxSizeEvent& event);
		void OnPaint(wxPaintEvent& event);

		void DrawColor() const;

	private:
		wxGLContext* m_context;

		d2d::Colorf m_col_begin, m_col_end;

		DECLARE_EVENT_TABLE()

	}; // Canvas

private:
	Canvas* m_canvas;

	DECLARE_EVENT_TABLE()

}; // GraduatedColorPanel

}

#endif // _DRAG2D_GRADUATED_COLOR_PANEL_H_