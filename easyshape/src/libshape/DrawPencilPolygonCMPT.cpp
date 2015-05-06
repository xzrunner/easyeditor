#include "DrawPencilPolygonCMPT.h"
#include "DrawPencilPolygonOP.h"

namespace libshape
{

DrawPencilPolygonCMPT::DrawPencilPolygonCMPT(wxWindow* parent, const wxString& name,
									   d2d::EditPanel* editPanel, 
									   d2d::MultiShapesImpl* shapesImpl)
	: d2d::OneFloatValueCMPT(parent, name, editPanel, "Simplify", 0, 100, 30, 0.1f)
{
	m_editOP = new DrawPencilPolygonOP(editPanel, shapesImpl, this, this); 
}

int DrawPencilPolygonCMPT::GetSelected() const
{
	return m_choice->GetSelection();
}

wxSizer* DrawPencilPolygonCMPT::initLayout()
{
	wxSizer* sizer = d2d::OneFloatValueCMPT::initLayout();

	sizer->AddSpacer(10);

	static const int SIZE = 5;
	wxString choices[SIZE];
	choices[0] = "normal";
	choices[1] = "union";
	choices[2] = "difference";
	choices[3] = "intersection";
	choices[4] = "xor";

	m_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, SIZE, choices);
	m_choice->SetSelection(0);
	sizer->Add(m_choice);

	return sizer;
}

}