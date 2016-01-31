#ifndef _EASYEDITOR_COLOR_SLIDER_H_
#define _EASYEDITOR_COLOR_SLIDER_H_

#include "Color.h"

#include <wx/wx.h>

namespace ee
{

class RGBColorPanel;
class ColorMonitor;
class TweenColorPanel;

class ColorSlider : public wxPanel
{
public:
	ColorSlider(wxWindow* parent, ColorMonitor* color_monitor, 
		const std::string& title, bool is_rgb);

	int GetColorValue() const;
	void SetColorValue(int col);

	void SetColorRegion(const Colorf& begin, const Colorf& end);

private:
	void InitLayout(const std::string& title, bool is_rgb);

	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxSpinEvent& event);

private:
	ColorMonitor* m_color_monitor;

	wxStaticText* m_title;

	TweenColorPanel* m_color_bg;
	
	wxSlider* m_slider;
	wxSpinCtrl* m_spin;

}; // wxPanel

}

#endif // _EASYEDITOR_COLOR_SLIDER_H_