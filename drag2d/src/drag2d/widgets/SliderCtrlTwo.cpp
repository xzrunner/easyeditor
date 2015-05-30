#include "SliderCtrlTwo.h"
#include "UICallback.h"

namespace d2d
{

SliderCtrlTwo::SliderCtrlTwo(wxPanel* parent, const char* title, const char* name, 
	UICallback* cb, int key, const SliderItem& item_a, const SliderItem& item_b,
	float scale_slider2text)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_cb(cb)
	, m_key(key)
	, m_scale_slider2text(scale_slider2text)
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

void SliderCtrlTwo::Load(const Json::Value& val, int version)
{
	int ivala, ivalb;
	if (version == 0) {
		ivala = val[m_name][m_item_a.name].asInt();
		ivalb = val[m_name][m_item_b.name].asInt();
	} else {
		ivala = val[m_name][m_item_a.name].asDouble() / m_scale_slider2text;
		ivalb = val[m_name][m_item_b.name].asDouble() / m_scale_slider2text;
	}
	m_item_a.slider->SetValue(ivala);
	m_item_b.slider->SetValue(ivalb);
}

void SliderCtrlTwo::Store(Json::Value& val)
{
	float fvala = m_item_a.slider->GetValue() * m_scale_slider2text,
		  fvalb = m_item_b.slider->GetValue() * m_scale_slider2text;
	val[m_name][m_item_a.name] = fvala;
	val[m_name][m_item_b.name] = fvalb;
}

void SliderCtrlTwo::Load()
{
	UICallback::Data data;
	m_cb->GetValue(m_key, data);
	float fvala = data.val0,
		  fvalb = data.val1;
	int ivala = (int)(fvala / m_scale_slider2text),
		ivalb = (int)(fvalb / m_scale_slider2text);
	m_item_a.slider->SetValue(ivala);
	m_item_b.slider->SetValue(ivalb);
	m_item_a.text->SetValue(wxString::FromDouble(fvala));
	m_item_b.text->SetValue(wxString::FromDouble(fvalb));
}

void SliderCtrlTwo::OnSetValue(wxScrollEvent& event)
{
	int ivala = m_item_a.slider->GetValue(),
		ivalb = m_item_b.slider->GetValue();
	float fvala = ivala * m_scale_slider2text,
		  fvalb = ivalb * m_scale_slider2text;
	m_item_a.text->SetValue(wxString::FromDouble(fvala));
	m_item_b.text->SetValue(wxString::FromDouble(fvalb));

	m_cb->SetValue(m_key, UICallback::Data(fvala, fvalb));
}

void SliderCtrlTwo::OnSetValue(wxCommandEvent& event)
{
	double fvala;
	m_item_a.text->GetValue().ToDouble(&fvala);
	double fvalb;
	m_item_b.text->GetValue().ToDouble(&fvalb);

	int ivala = (int)(fvala / m_scale_slider2text),
		ivalb = (int)(fvalb / m_scale_slider2text);

	m_item_a.slider->SetValue(ivala);
	m_item_b.slider->SetValue(ivalb);

	m_cb->SetValue(m_key, UICallback::Data(fvala, fvalb));
}

}