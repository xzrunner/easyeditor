#ifndef _EASYEDITOR_RGB_COLOR_PANEL_H_
#define _EASYEDITOR_RGB_COLOR_PANEL_H_

#include "TweenColorPanel.h"
#include "SimpleGLCanvas.h"
#include "Color.h"

#include <wx/wx.h>

namespace ee
{

class RGBColorPanel : public wxPanel, public TweenColorPanel
{
public:
	RGBColorPanel(wxWindow* parent, wxSize size);
	virtual ~RGBColorPanel();

	//
	// interface TweenColorPanel
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

#endif // _EASYEDITOR_RGB_COLOR_PANEL_H_