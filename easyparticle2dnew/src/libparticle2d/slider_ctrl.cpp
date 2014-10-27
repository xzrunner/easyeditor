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
		wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
	Connect(m_slider->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(SliderCtrlOne::OnSetValue));
	sizer->Add(m_slider);

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
	m_cb->OnSetKeyValue(m_key, m_slider->GetValue());
}

//////////////////////////////////////////////////////////////////////////
// class SliderWithOffsetCtrl
//////////////////////////////////////////////////////////////////////////

SliderCtrlTwo::SliderCtrlTwo(wxPanel* parent, const char* title, const char* name, 
	UICallback* cb, int key,
	const SliderItem& item0, const SliderItem& item1)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_cb(cb)
	, m_key(key)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, item0.title));

		m_item0 = new wxSlider(this, wxID_ANY, item0.default, item0.min, item0.max, 
			wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_item0->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(SliderCtrlTwo::OnSetValue));
		sizer->Add(m_item0);
		m_item0_name = item0.name;

		top_sizer->Add(sizer);
	}
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, item1.title));

		m_item1 = new wxSlider(this, wxID_ANY, item1.default, item1.min, item1.max, 
			wxDefaultPosition, wxSize(200, -1), wxSL_VALUE_LABEL);
		Connect(m_item1->GetId(), wxEVT_SCROLL_CHANGED, wxScrollEventHandler(SliderCtrlTwo::OnSetValue));
		sizer->Add(m_item1);
		m_item1_name = item1.name;

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
	m_item0->SetValue(val[m_name][m_item0_name].asInt());
	m_item1->SetValue(val[m_name][m_item1_name].asInt());
}

void SliderCtrlTwo::Store(Json::Value& val)
{
	val[m_name][m_item0_name] = m_item0->GetValue();
	val[m_name][m_item1_name] = m_item1->GetValue();
}

void SliderCtrlTwo::OnSetValue(wxScrollEvent& event)
{
	m_cb->OnSetKeyValue(m_key, m_item0->GetValue(), m_item1->GetValue());
}

}