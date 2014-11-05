#include "SliderCtrlOne.h"
#include "UICallback.h"
#include "ParticleSystem.h"

namespace eparticle2d
{

SliderCtrlOne::SliderCtrlOne(wxPanel* parent, const char* title, const char* name, 
	UICallback* cb, int key, 
	const SliderItem& item)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_cb(cb)
	, m_key(key)
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

void SliderCtrlOne::Load(const Json::Value& val)
{
	m_slider->SetValue(val[m_name].asInt());
}

void SliderCtrlOne::Store(Json::Value& val)
{
	val[m_name] = m_slider->GetValue();
}

void SliderCtrlOne::Load()
{
	UICallback::Data data;
	m_cb->GetValue(m_key, data);
	m_slider->SetValue(data.val0);
	m_text->SetValue(wxString::FromDouble(data.val0));
}

void SliderCtrlOne::OnSetValue(wxScrollEvent& event)
{
	int val = m_slider->GetValue();
	m_text->SetValue(wxString::FromDouble(val));
	m_cb->SetValue(m_key, UICallback::Data(val));
}

void SliderCtrlOne::OnSetValue(wxCommandEvent& event)
{
	double val;
	m_text->GetValue().ToDouble(&val);
	m_slider->SetValue(val);
	m_cb->SetValue(m_key, UICallback::Data(val));
}

}