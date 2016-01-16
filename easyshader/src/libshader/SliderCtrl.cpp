#include "SliderCtrl.h"
#include "Uniform.h"
#include "Shader.h"

namespace eshader
{

SliderCtrl::SliderCtrl(wxPanel* parent, const std::string& title, const std::string& name, 
					   Shader* shader, Uniform* uniform, 
					   const std::vector<SliderItemInt>& items, float slider_accuracy,
					   d2d::IStageCanvas* canvas)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_slider_accuracy(slider_accuracy)
	, m_shader(shader)
	, m_uniform(uniform)
	, m_canvas(canvas)
{
	InitLayout(title, items);
}

SliderCtrl::SliderCtrl(wxPanel* parent, const std::string& title, const std::string& name, 
					   Shader* shader, Uniform* uniform, 
					   const std::vector<SliderItemFloat>& items, float slider_accuracy,
					   d2d::IStageCanvas* canvas)
	: wxPanel(parent, wxID_ANY)
	, m_name(name)
	, m_slider_accuracy(slider_accuracy)
	, m_shader(shader)
	, m_uniform(uniform)
	, m_canvas(canvas)
{
	InitLayout(title, items);
}

void SliderCtrl::GetValue(double values[16]) const
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		m_items[i].text->GetValue().ToDouble(&values[i]);
	}
}

template <typename T>
void SliderCtrl::InitLayout(const std::string& title, 
							const std::vector<SliderItem<T> >& items)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	for (int i = 0, n = items.size(); i < n; ++i)
	{
		const SliderItem<T>& src = items[i];
		Item dst;

		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, src.title, wxDefaultPosition, wxSize(40, -1)));

		dst.slider = new wxSlider(this, wxID_ANY, src.default / m_slider_accuracy, src.min / m_slider_accuracy, 
			src.max / m_slider_accuracy, wxDefaultPosition, wxSize(160, -1));
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
		float fval = ival * m_slider_accuracy;
		item.text->SetValue(wxString::FromDouble(fval));
	}

	UpdateUniformValue();
}

void SliderCtrl::OnSetValue(wxCommandEvent& event)
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		const Item& item = m_items[i];
		double fval;
		item.text->GetValue().ToDouble(&fval);
		int ival = (int)(fval / m_slider_accuracy);
		item.slider->SetValue(ival);
	}

	UpdateUniformValue();
}

void SliderCtrl::UpdateUniformValue()
{
	double values[16];
	GetValue(values);

	switch (m_uniform->GetType())
	{
	case UT_INT: case UT_BOOL:
		m_uniform->Set((int)values[0]);
		break;
	case UT_FLOAT:
		m_uniform->Set((float)values[0]);
		break;
	case UT_IVEC2:
		m_uniform->Set((int)values[0], (int)values[1]);
		break;
	case UT_IVEC3:
		m_uniform->Set((int)values[0], (int)values[1], (int)values[2]);
		break;
	case UT_IVEC4:
		m_uniform->Set((int)values[0], (int)values[1], (int)values[2], (int)values[3]);
		break;
	case UT_VEC2:
		m_uniform->Set((float)values[0], (float)values[1]);
		break;
	case UT_VEC3:
		m_uniform->Set((float)values[0], (float)values[1], (float)values[2]);
		break;
	case UT_VEC4:
		m_uniform->Set((float)values[0], (float)values[1], (float)values[2], (float)values[3]);
		break;
	}

	d2d::ShaderMgr::Instance()->sprite();
	m_uniform->Load();

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

}