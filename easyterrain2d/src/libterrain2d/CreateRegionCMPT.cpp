#include "CreateRegionCMPT.h"
#include "StagePanel.h"
#include "CreateRegionOP.h"

namespace eterrain2d
{

CreateRegionCMPT::CreateRegionCMPT(wxWindow* parent, const wxString& name, StagePanel* stage)
	: d2d::OneFloatValueCMPT(parent, name, stage, "node capture", 5, 30, 10)
{
	m_editOP = new libshape::EditPolylineOP<CreateRegionOP, d2d::SelectShapesOP>(stage, stage, NULL, NULL, this, this);
}

// float CreateRegionCMPT::GetValue() const
// {
// 	return 5;
// }

wxSizer* CreateRegionCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

 	wxSizer* psizer = d2d::OneFloatValueCMPT::initLayout();
 	sizer->Add(psizer);
	sizer->AddSpacer(20);
	
	wxCheckBox* tris_edge = new wxCheckBox(this, wxID_ANY, wxT("triangle edge"));
	tris_edge->SetValue(d2d::Settings::bDisplayTrisEdge);
	Connect(tris_edge->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
		wxCommandEventHandler(CreateRegionCMPT::OnChangeDisplayTriangles));
	sizer->Add(tris_edge);

	return sizer;
}

void CreateRegionCMPT::OnChangeDisplayTriangles(wxCommandEvent& event)
{
	d2d::Settings::bDisplayTrisEdge = event.IsChecked();
	m_stage->SetCanvasDirty();
}

}