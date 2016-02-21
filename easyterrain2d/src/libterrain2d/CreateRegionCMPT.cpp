#include "CreateRegionCMPT.h"
#include "StagePanel.h"
#include "CreateRegionOP.h"

#include <ee/SelectShapesOP.h>
#include <ee/SettingData.h>
#include <ee/panel_msg.h>

namespace eterrain2d
{

CreateRegionCMPT::CreateRegionCMPT(wxWindow* parent, const std::string& name, StagePanel* stage)
	: ee::OneFloatValueCMPT(parent, name, stage->GetStageImpl(), "node capture", 5, 30, 10)
{
	m_editop = new eshape::EditPolylineOP<CreateRegionOP, ee::SelectShapesOP>(stage, stage->GetStageImpl(), stage, NULL, this, this);
}

// float CreateRegionCMPT::GetValue() const
// {
// 	return 5;
// }

wxSizer* CreateRegionCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

 	wxSizer* psizer = ee::OneFloatValueCMPT::InitLayout();
 	sizer->Add(psizer);
	sizer->AddSpacer(20);
	
	wxCheckBox* tris_edge = new wxCheckBox(this, wxID_ANY, wxT("triangle edge"));
	tris_edge->SetValue(ee::SettingData::draw_tris_edge);
	Connect(tris_edge->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
		wxCommandEventHandler(CreateRegionCMPT::OnChangeDisplayTriangles));
	sizer->Add(tris_edge);

	return sizer;
}

void CreateRegionCMPT::OnChangeDisplayTriangles(wxCommandEvent& event)
{
	ee::SettingData::draw_tris_edge = event.IsChecked();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}