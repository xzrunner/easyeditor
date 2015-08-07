#include "OneFloatValueCMPT.h"

namespace d2d
{

OneFloatValueCMPT::OneFloatValueCMPT(wxWindow* parent, const wxString& name, 
									 EditPanelImpl* stage, const std::string& title, 
									 int min, int max, int value, float scale)
	: AbstractEditCMPT(parent, name, stage)
	, m_title(title)
	, m_min(min)
	, m_max(max)
	, m_value(value)
	, m_scale(scale)
	, m_slider(NULL)
{
}

float OneFloatValueCMPT::GetValue() const
{
	return m_slider->GetValue() * m_scale;
}

wxSizer* OneFloatValueCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, m_title);
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	m_slider = new wxSlider(this, wxID_ANY, m_value, m_min, m_max, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
	sizer->Add(m_slider);
	return sizer;
}

}