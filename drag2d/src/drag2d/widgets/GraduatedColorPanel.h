#ifndef _DRAG2D_GRADUATED_COLOR_PANEL_H_
#define _DRAG2D_GRADUATED_COLOR_PANEL_H_

#include "SimpleGLCanvas.h"

#include "common/Color.h"

#include <wx/wx.h>

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
	class Canvas : public SimpleGLCanvas
	{
	public:
		Canvas(wxWindow* parent);

		void SetColor(const d2d::Colorf& begin, const d2d::Colorf& end);

	protected:
		virtual void OnDraw() const;

	private:
		d2d::Colorf m_col_begin, m_col_end;

	}; // Canvas

private:
	Canvas* m_canvas;

	DECLARE_EVENT_TABLE()

}; // GraduatedColorPanel

}

#endif // _DRAG2D_GRADUATED_COLOR_PANEL_H_