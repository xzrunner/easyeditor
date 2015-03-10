#ifndef _DRAG2D_RGB_COLOR_PANEL_H_
#define _DRAG2D_RGB_COLOR_PANEL_H_

#include "ITweenColorPanel.h"
#include "SimpleGLCanvas.h"

#include "common/Color.h"

#include <wx/wx.h>

namespace d2d
{

class RGBColorPanel : public wxPanel, public ITweenColorPanel
{
public:
	RGBColorPanel(wxWindow* parent, wxSize size);
	virtual ~RGBColorPanel();

	//
	// interface ITweenColorPanel
	//
	virtual void SetColor(const Colorf& begin, const Colorf& end);

private:
	void OnSize(wxSizeEvent& event);

private:
	class Canvas : public SimpleGLCanvas
	{
	public:
		Canvas(wxWindow* parent);

		void SetColor(const Colorf& begin, const Colorf& end);

	protected:
		virtual void OnDraw() const;

	private:
		Colorf m_col_begin, m_col_end;

	}; // Canvas

private:
	Canvas* m_canvas;

	DECLARE_EVENT_TABLE()

}; // RGBColorPanel

}

#endif // _DRAG2D_RGB_COLOR_PANEL_H_