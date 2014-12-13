#ifndef _ESHADER_SLIDER_CTRL_H_
#define _ESHADER_SLIDER_CTRL_H_

#include <wx/wx.h>
#include <vector>

#include "SliderItem.h"

namespace eshader 
{

class Uniform;

class SliderCtrl : public wxPanel
{
public:
	SliderCtrl(wxPanel* parent, const std::string& title, const std::string& name,
		Uniform* uniform, const std::vector<SliderItemFloat>& items, float scale_slider2text = 1);

//	void Update();

private:
	void InitLayout(const std::string& title, const std::vector<SliderItemFloat>& items);

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

	float m_scale_slider2text;

	std::vector<Item> m_items;

	Uniform* m_uniform;

}; // SliderCtrl

}

#endif // _ESHADER_SLIDER_CTRL_H_