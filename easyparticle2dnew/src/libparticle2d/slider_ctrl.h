#ifndef _EASYPARTICLE2D_SLIDER_CTRL_H_
#define _EASYPARTICLE2D_SLIDER_CTRL_H_

#include "ISliderCtrl.h"

namespace eparticle2d
{

class UICallback;

struct SliderItem
{
	std::string title;
	std::string name;

	int default;
	int min;
	int max;

	SliderItem(const char* title, const char* name, int default, int min, int max) 
		: title(title), name(name), default(default), min(min), max(max) {}

}; // SliderItem

class SliderCtrlOne : public ISliderCtrl, public wxPanel
{
public:
	SliderCtrlOne(wxPanel* parent, const char* title, const char* name, 
		UICallback* cb, int key,
		const SliderItem& item);

	virtual void Update();
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val);

private:
	void OnSetValue(wxScrollEvent& event);

private:
	std::string m_name;

	UICallback* m_cb;
	int m_key;

	wxSlider* m_slider;

}; // SliderCtrlOne

class SliderCtrlTwo : public ISliderCtrl, public wxPanel
{
public:
	SliderCtrlTwo(wxPanel* parent, const char* title, const char* name, 
		UICallback* cb, int key,
		const SliderItem& item0, const SliderItem& item1);

	virtual void Update();
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val);

private:
	void OnSetValue(wxScrollEvent& event);

private:
	std::string m_name;

	UICallback* m_cb;
	int m_key;

	wxSlider *m_item0, *m_item1;
	std::string m_item0_name, m_item1_name;

}; // SliderCtrlTwo

}

#endif // _EASYPARTICLE2D_SLIDER_CTRL_H_