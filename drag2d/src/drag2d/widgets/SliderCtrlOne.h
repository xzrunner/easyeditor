#ifndef _DRAG2D_SLIDER_CTRL_ONE_H_
#define _DRAG2D_SLIDER_CTRL_ONE_H_

#include "ISliderCtrl.h"
#include "SliderItem.h"

#include <wx/wx.h>

namespace d2d
{

class UICallback;

class SliderCtrlOne : public ISliderCtrl, public wxPanel
{
public:
	SliderCtrlOne(wxPanel* parent, const char* title, const char* name, 
		UICallback* cb, int key, const SliderItem& item, float scale_slider2text = 1);

	virtual void Update();
	virtual void Load(const Json::Value& val, int version);
	virtual void Store(Json::Value& val);
	virtual void Load();

private:
	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxCommandEvent& event);

private:
	std::string m_name;

	UICallback* m_cb;
	int m_key;

	float m_scale_slider2text;

	wxSlider* m_slider;
	wxTextCtrl* m_text;

}; // SliderCtrlOne

}

#endif // _DRAG2D_SLIDER_CTRL_ONE_H_