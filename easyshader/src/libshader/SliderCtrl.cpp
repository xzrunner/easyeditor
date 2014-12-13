#include "SliderCtrl.h"
#include "Uniform.h"

namespace eshader
{

SliderCtrl::SliderCtrl(wxPanel* parent, const std::string& title, const std::string& name, 
					   Uniform* uniform, const std::vector<SliderItemFloat>& items, float scale_slider2text)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_scale_slider2text(scale_slider2text)
	, m_uniform(uniform)
{
	InitLayout(title, items);
}

// template<class T>
// void SliderCtrl<T>::Update()
// {
// 	
// }

void SliderCtrl::InitLayout(const std::string& title, 
							const std::vector<SliderItemFloat>& items)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	for (int i = 0, n = items.size(); i < n; ++i)
	{
		const SliderItemFloat& src = items[i];
		Item dst;

		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, src.title, wxDefaultPosition, wxSize(40, -1)));

		dst.slider = new wxSlider(this, wxID_ANY, src.default / m_scale_slider2text, src.min / m_scale_slider2text, 
			src.max / m_scale_slider2text, wxDefaultPosition, wxSize(160, -1));
		Connect(dst.slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(SliderCtrl::OnSetValue));
		sizer->Add(dst.slider);

		dst.text = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(src.default), 
			wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
		Connect(dst.text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SliderCtrl::OnSetValue));
		sizer->Add(dst.text);

		m_items.push_back(dst);

		top_sizer->Add(sizer);
	}
	SetSizer(top_sizer);
}

void SliderCtrl::OnSetValue(wxScrollEvent& event)
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		const Item& item = m_items[i];
		int ival = item.slider->GetValue();
		float fval = ival * m_scale_slider2text;
		item.text->SetValue(wxString::FromDouble(fval));

		// todo temp
		m_uniform->Set(fval);
	}

	m_uniform->Load();
}

void SliderCtrl::OnSetValue(wxCommandEvent& event)
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		const Item& item = m_items[i];
		double fval;
		item.text->GetValue().ToDouble(&fval);
		int ival = (int)(fval / m_scale_slider2text);
		item.slider->SetValue(ival);

		// todo temp
		m_uniform->Set((float)fval);
	}

	m_uniform->Load();
}

}