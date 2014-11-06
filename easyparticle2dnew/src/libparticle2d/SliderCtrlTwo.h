#ifndef _EASYPARTICLE2D_SLIDER_CTRL_TWO_H_
#define _EASYPARTICLE2D_SLIDER_CTRL_TWO_H_

#include "ISliderCtrl.h"
#include "SliderItem.h"

namespace eparticle2d
{

class UICallback;

class SliderCtrlTwo : public ISliderCtrl, public wxPanel
{
public:
	SliderCtrlTwo(wxPanel* parent, const char* title, const char* name, 
		UICallback* cb, int key, const SliderItem& item_a, const SliderItem& item_b,
		float scale_slider2text = 1);

	virtual void Update();
	virtual void Load(const Json::Value& val, int version);
	virtual void Store(Json::Value& val);
	virtual void Load();

private:
	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxCommandEvent& event);

private:
	struct Item
	{
		wxSlider* slider;
		wxTextCtrl* text;
		std::string name;
	};

private:
	std::string m_name;

	UICallback* m_cb;
	int m_key;

	float m_scale_slider2text;

	Item m_item_a, m_item_b;

}; // SliderCtrlTwo

}

#endif // _EASYPARTICLE2D_SLIDER_CTRL_TWO_H_