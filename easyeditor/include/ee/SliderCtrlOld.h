#ifndef _EASYEDITOR_SLIDER_CTRL_H_
#define _EASYEDITOR_SLIDER_CTRL_H_

#include <wx/wx.h>

namespace ee
{

class SliderCtrlOld : public wxPanel
{
public:
	SliderCtrlOld(wxPanel* parent, const char* title, const char* name, 
		int val, int min, int max, float scale_slider2text = 1);

private:
	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxCommandEvent& event);

private:
	std::string m_name;

	float m_scale_slider2text;

	wxSlider* m_slider;
	wxTextCtrl* m_text;

}; // SliderCtrlOld

}

#endif // _EASYEDITOR_SLIDER_CTRL_H_