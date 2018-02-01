#ifndef _EASYEDITOR_RGB_COLOR_PANEL_H_
#define _EASYEDITOR_RGB_COLOR_PANEL_H_

#include "TweenColorPanel.h"
#include "SimpleGLCanvas.h"

#include <wx/wx.h>

#include <memory>

namespace ee
{

class RGBColorPanel : public wxPanel, public TweenColorPanel
{
public:
	RGBColorPanel(wxWindow* parent, wxSize size);

	//
	// interface TweenColorPanel
	//
	virtual void SetColor(const pt2::Color& begin, const pt2::Color& end) override;

private:
	void OnSize(wxSizeEvent& event);

private:
	class Canvas : public SimpleGLCanvas
	{
	public:
		Canvas(wxWindow* parent);

		void SetColor(const pt2::Color& begin, const pt2::Color& end);

	protected:
		virtual void OnDraw() const override;

	private:
		pt2::Color m_col_begin, m_col_end;

	}; // Canvas

private:
	std::shared_ptr<Canvas> m_canvas = nullptr;

	DECLARE_EVENT_TABLE()

}; // RGBColorPanel

}

#endif // _EASYEDITOR_RGB_COLOR_PANEL_H_