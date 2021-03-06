#ifndef _EASYEDITOR_SLIDER_CTRL_ONE_H_
#define _EASYEDITOR_SLIDER_CTRL_ONE_H_

#include "SliderCtrl.h"
#include "SliderItem.h"

#include <wx/wx.h>

namespace ee
{

class UICallback;

class SliderCtrlOne : public SliderCtrl, public wxPanel
{
public:
	SliderCtrlOne(wxPanel* parent, const char* title, const char* name, 
		UICallback* cb, int key, const SliderItem& item, float scale_slider2text = 1);

	virtual void Update() override;
	virtual void Load(const Json::Value& val, int version) override;
	virtual void Store(Json::Value& val) override;
	virtual void Load() override;

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

#endif // _EASYEDITOR_SLIDER_CTRL_ONE_H_