#include "SliderCtrlOne.h"
#include "UICallback.h"
#include "ParticleSystem.h"

namespace eparticle2d
{

SliderCtrlOne::SliderCtrlOne(wxPanel* parent, const char* title, const char* name, 
	UICallback* cb, int key, const SliderItem& item, float scale_slider2text)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_cb(cb)
	, m_key(key)
	, m_scale_slider2text(scale_slider2text)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(new wxStaticText(this, wxID_ANY, item.title));

	m_slider = new wxSlider(this, wxID_ANY, item.default, item.min, item.max, 
		wxDefaultPosition, wxSize(200, -1));
	Connect(m_slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(SliderCtrlOne::OnSetValue));
	sizer->Add(m_slider);

	m_text = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(item.default), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
	Connect(m_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SliderCtrlOne::OnSetValue));
	sizer->Add(m_text);

	top_sizer->Add(sizer);

	SetSizer(top_sizer);
}

void SliderCtrlOne::Update()
{
	OnSetValue(wxScrollEvent());
}

void SliderCtrlOne::Load(const Json::Value& val, int version)
{
	int ival;
	if (version == 0) {
		ival = val[m_name].asInt();
	} else {
		ival = val[m_name].asDouble() / m_scale_slider2text;
	}
	m_slider->SetValue(ival);
}

void SliderCtrlOne::Store(Json::Value& val)
{
	float fval = m_slider->GetValue() * m_scale_slider2text;
	val[m_name] = fval;
}

void SliderCtrlOne::Load()
{
	UICallback::Data data;
	m_cb->GetValue(m_key, data);
	float fval = data.val0;
	int ival = (int)(fval / m_scale_slider2text);
	m_slider->SetValue(ival);
	m_text->SetValue(wxString::FromDouble(fval));
}

void SliderCtrlOne::OnSetValue(wxScrollEvent& event)
{
	int ival = m_slider->GetValue();
	float fval = ival * m_scale_slider2text;
	m_text->SetValue(wxString::FromDouble(fval));
	m_cb->SetValue(m_key, UICallback::Data(fval));
}

void SliderCtrlOne::OnSetValue(wxCommandEvent& event)
{
	double fval;
	m_text->GetValue().ToDouble(&fval);
	int ival = (int)(fval / m_scale_slider2text);
	m_slider->SetValue(ival);
	m_cb->SetValue(m_key, UICallback::Data(fval));
}

}