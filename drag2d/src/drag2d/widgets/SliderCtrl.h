#ifndef _DRAG2D_SLIDER_CTRL_H_
#define _DRAG2D_SLIDER_CTRL_H_

#include <wx/wx.h>

namespace d2d
{

class SliderCtrl : public wxPanel
{
public:
	SliderCtrl(wxPanel* parent, const char* title, const char* name, 
		int val, int min, int max, float scale_slider2text = 1);

private:
	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxCommandEvent& event);

private:
	std::string m_name;

	float m_scale_slider2text;

	wxSlider* m_slider;
	wxTextCtrl* m_text;

}; // SliderCtrl

}

#endif // _DRAG2D_SLIDER_CTRL_H_