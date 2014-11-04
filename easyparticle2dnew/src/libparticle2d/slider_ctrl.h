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
	void OnSetValue(wxCommandEvent& event);

private:
	std::string m_name;

	UICallback* m_cb;
	int m_key;

	wxSlider* m_slider;
	wxTextCtrl* m_text;

}; // SliderCtrlOne

class SliderCtrlTwo : public ISliderCtrl, public wxPanel
{
public:
	SliderCtrlTwo(wxPanel* parent, const char* title, const char* name, 
		UICallback* cb, int key,
		const SliderItem& item_a, const SliderItem& item_b);

	virtual void Update();
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val);

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

	Item m_item_a, m_item_b;

}; // SliderCtrlTwo

}

#endif // _EASYPARTICLE2D_SLIDER_CTRL_H_