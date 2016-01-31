#include "SliderCtrlOld.h"
#include "StringHelper.h"

namespace ee
{

SliderCtrlOld::SliderCtrlOld(wxPanel* parent, const char* title, const char* name, 
							 int val, int min, int max, float scale_slider2text)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_scale_slider2text(scale_slider2text)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(new wxStaticText(this, wxID_ANY, title));

	m_slider = new wxSlider(this, wxID_ANY, val, min, max, wxDefaultPosition, wxSize(200, -1));
	Connect(m_slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(SliderCtrlOld::OnSetValue));
	sizer->Add(m_slider);

	m_text = new wxTextCtrl(this, wxID_ANY, StringHelper::ToString(val), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
	Connect(m_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SliderCtrlOld::OnSetValue));
	sizer->Add(m_text);

	top_sizer->Add(sizer);

	SetSizer(top_sizer);
}

void SliderCtrlOld::OnSetValue(wxScrollEvent& event)
{
	int ival = m_slider->GetValue();
	float fval = ival * m_scale_slider2text;
	m_text->SetValue(StringHelper::ToString(fval));
}

void SliderCtrlOld::OnSetValue(wxCommandEvent& event)
{
	double fval;
	m_text->GetValue().ToDouble(&fval);
	int ival = (int)(fval / m_scale_slider2text);
	m_slider->SetValue(ival);
}

}