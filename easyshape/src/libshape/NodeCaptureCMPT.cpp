#include "NodeCaptureCMPT.h"

namespace libshape
{

NodeCaptureCMPT::NodeCaptureCMPT(wxWindow* parent, const wxString& name, d2d::EditPanel* editPanel)
	: AbstractEditCMPT(parent, name, editPanel)
	, m_slider(NULL)
{
//	m_editOP = new T(editPanel, shapesImpl, propertyPanel, this);
}

float NodeCaptureCMPT::GetScope() const
{
	return m_slider->GetValue();
}

wxSizer* NodeCaptureCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Node Capture"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	m_slider = new wxSlider(this, wxID_ANY, 5, 0, 15, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
	sizer->Add(m_slider);
	return sizer;
}

}