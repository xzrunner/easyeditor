#ifndef _EASYEDITOR_HSL_COLOR_PANEL_H_
#define _EASYEDITOR_HSL_COLOR_PANEL_H_

#include "TweenColorPanel.h"
#include "SimpleGLCanvas.h"
#include "Color.h"

#include <wx/wx.h>

namespace ee
{

class HSLColorPanel : public wxPanel, public TweenColorPanel
{
public:
	HSLColorPanel(wxWindow* parent, wxSize size);
	virtual ~HSLColorPanel();

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

}; // HSLColorPanel

}

#endif // _EASYEDITOR_HSL_COLOR_PANEL_H_