#include "slider_ctrl.h"
#include "UICallback.h"

namespace eparticle2d
{

//////////////////////////////////////////////////////////////////////////
// class SliderCtrl
//////////////////////////////////////////////////////////////////////////

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

void SliderCtrlOne::OnSetValue(wxScrollEvent& event)
{
	int val = m_slider->GetValue();
	m_text->SetValue(wxString::FromDouble(val));
	m_cb->OnSetKeyValue(m_key, val);
}

void SliderCtrlOne::OnSetValue(wxCommandEvent& event)
{
	double val;
	m_text->GetValue().ToDouble(&val);
	m_slider->SetValue(val);
	m_cb->OnSetKeyValue(m_key, val);
}

//////////////////////////////////////////////////////////////////////////
// class SliderWithOffsetCtrl
//////////////////////////////////////////////////////////////////////////

SliderCtrlTwo::SliderCtrlTwo(wxPanel* parent, const char* title, const char* name, 
	UICallback* cb, int key,
	const SliderItem& item_a, const SliderItem& item_b)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_cb(cb)
	, m_key(key)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, item_a.title, wxDefaultPosition, wxSize(40, -1)));

		m_item_a.slider = new wxSlider(this, wxID_ANY, item_a.default, item_a.min, item_a.max, 
			wxDefaultPosition, wxSize(160, -1));
		Connect(m_item_a.slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(SliderCtrlTwo::OnSetValue));
		sizer->Add(m_item_a.slider);

		m_item_a.text = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(item_a.default), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
		Connect(m_item_a.text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SliderCtrlTwo::OnSetValue));
		sizer->Add(m_item_a.text);

		m_item_a.name = item_a.name;

		top_sizer->Add(sizer);
	}
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, item_b.title, wxDefaultPosition, wxSize(40, -1)));

		m_item_b.slider = new wxSlider(this, wxID_ANY, item_b.default, item_b.min, item_b.max, 
			wxDefaultPosition, wxSize(160, -1));
		Connect(m_item_b.slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(SliderCtrlTwo::OnSetValue));
		sizer->Add(m_item_b.slider);

		m_item_b.text = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(item_b.default), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
		Connect(m_item_b.text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SliderCtrlTwo::OnSetValue));
		sizer->Add(m_item_b.text);

		m_item_b.name = item_b.name;

		top_sizer->Add(sizer);
	}
	SetSizer(top_sizer);
}

void SliderCtrlTwo::Update()
{
	OnSetValue(wxScrollEvent());
}

void SliderCtrlTwo::Load(const Json::Value& val)
{
	m_item_a.slider->SetValue(val[m_name][m_item_a.name].asInt());
	m_item_b.slider->SetValue(val[m_name][m_item_b.name].asInt());
}

void SliderCtrlTwo::Store(Json::Value& val)
{
	val[m_name][m_item_a.name] = m_item_a.slider->GetValue();
	val[m_name][m_item_b.name] = m_item_b.slider->GetValue();
}

void SliderCtrlTwo::OnSetValue(wxScrollEvent& event)
{
	int val_a = m_item_a.slider->GetValue(),
		val_b = m_item_b.slider->GetValue();
	m_item_a.text->SetValue(wxString::FromDouble(val_a));
	m_item_b.text->SetValue(wxString::FromDouble(val_b));

	m_cb->OnSetKeyValue(m_key, val_a, val_b);
}

void SliderCtrlTwo::OnSetValue(wxCommandEvent& event)
{
	double val;
	m_item_a.text->GetValue().ToDouble(&val);
	int val_a = (int)val;
	m_item_b.text->GetValue().ToDouble(&val);
	int val_b = (int)val;

	m_item_a.slider->SetValue(val_a);
	m_item_b.slider->SetValue(val_b);

	m_cb->OnSetKeyValue(m_key, val_a, val_b);
}

}