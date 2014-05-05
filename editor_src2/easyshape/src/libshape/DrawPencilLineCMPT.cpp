#include "DrawPencilLineCMPT.h"
#include "DrawPencilLineOP.h"

namespace libshape
{

DrawPencilLineCMPT::DrawPencilLineCMPT(wxWindow* parent, const wxString& name,
									   d2d::EditPanel* editPanel, 
									   d2d::MultiShapesImpl* shapesImpl)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new DrawPencilLineOP(editPanel, shapesImpl, this); 
}

wxSizer* DrawPencilLineCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Simplify"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	m_thresholdSlider = new wxSlider(this, wxID_ANY, 30, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
	sizer->Add(m_thresholdSlider);
	return sizer;
}

}