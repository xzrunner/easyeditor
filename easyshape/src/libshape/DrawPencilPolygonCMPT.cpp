#include "DrawPencilPolygonCMPT.h"
#include "DrawPencilPolygonOP.h"

namespace libshape
{

DrawPencilPolygonCMPT::DrawPencilPolygonCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd, 
											 ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl)
	: ee::OneFloatValueCMPT(parent, name, stage, "Simplify", 0, 100, 30, 0.1f)
{
	m_editOP = new DrawPencilPolygonOP(stage_wnd, stage, shapesImpl, this, this); 
}

int DrawPencilPolygonCMPT::GetSelected() const
{
	return m_choice->GetSelection();
}

void DrawPencilPolygonCMPT::SetSelection(const std::string& type)
{
	if (type == "normal") {
		m_choice->SetSelection(0);
	} else if (type == "union") {
		m_choice->SetSelection(1);
	} else if (type == "difference") {
		m_choice->SetSelection(2);		
	}
}

wxSizer* DrawPencilPolygonCMPT::InitLayout()
{
	wxSizer* sizer = ee::OneFloatValueCMPT::InitLayout();

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