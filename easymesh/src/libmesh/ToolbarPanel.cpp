#include "ToolbarPanel.h"
#include "CreateMeshCMPT.h"
#include "EditMeshCMPT.h"
#include "EditUVCMPT.h"
#include "StagePanel.h"
#include "Sprite.h"
#include "MeshFactory.h"

namespace emesh
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage, bool full, Sprite* spr)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
	, m_spr(spr)
	, m_create_cmpt(NULL)
	, m_edit_cmpt(NULL)
{
	if (full)
	{
		AddChild(m_create_cmpt = new CreateMeshCMPT(this, "Create", stage));
	}
	AddChild(m_edit_cmpt = new EditMeshCMPT(this, "Edit", stage));
	AddChild(new EditUVCMPT(this, "UV", stage));
	SetSizer(InitLayout());	
}

void ToolbarPanel::SetEditType(gum::MeshType type)
{
	MeshFactory::Instance()->SetShapeType(type);
	if (m_create_cmpt) {
		m_create_cmpt->SetEditOP(type);
	}
	if (m_edit_cmpt) {
		m_edit_cmpt->SetEditOP(type);
	}
	m_stage->SetEditOP(GetChildEditOP());

	m_type_choice->SetSelection(type);
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(InitChildrenLayout());

	sizer->AddSpacer(15);
	{
		wxArrayString choices;
		choices.Add(wxT("network"));
		choices.Add(wxT("strip"));
		choices.Add(wxT("skeleton"));
		m_type_choice = new wxRadioBox(this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, choices, 3, wxRA_SPECIFY_ROWS);
		m_type_choice->SetSelection(0);
		MeshFactory::Instance()->SetShapeType(gum::MESH_NETWORK);
		Connect(m_type_choice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(ToolbarPanel::OnChangeType));
		sizer->Add(m_type_choice);
	}
		
// 	sizer->AddSpacer(10);
// 	{
// 		wxSizer* speed_sizer = new wxBoxSizer(wxHORIZONTAL);
// 		speed_sizer->Add(new wxStaticText(this, wxID_ANY, "speed x"), 0, wxLEFT | wxRIGHT, 5);
// 
// 		m_ctrl_xspeed = new wxTextCtrl(this, wxID_ANY);
// 		Connect(m_ctrl_xspeed->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ToolbarPanel::OnSetSpeed));
// 		if (m_spr) {
// 			m_ctrl_xspeed->SetValue(wxString::FromDouble(m_spr->GetSpeed().x));
// 		}
// 		speed_sizer->Add(m_ctrl_xspeed, 0, wxLEFT | wxRIGHT, 5);
// 		sizer->Add(speed_sizer);
// 	}
// 	{
// 		wxSizer* speed_sizer = new wxBoxSizer(wxHORIZONTAL);
// 		speed_sizer->Add(new wxStaticText(this, wxID_ANY, "speed y"), 0, wxLEFT | wxRIGHT, 5);
// 
// 		m_ctrl_yspeed = new wxTextCtrl(this, wxID_ANY);
// 		Connect(m_ctrl_yspeed->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ToolbarPanel::OnSetSpeed));
// 		if (m_spr) {
// 			m_ctrl_yspeed->SetValue(wxString::FromDouble(m_spr->GetSpeed().y));
// 		}
// 		speed_sizer->Add(m_ctrl_yspeed, 0, wxLEFT | wxRIGHT, 5);
// 		sizer->Add(speed_sizer);
// 	}
	return sizer;
}

void ToolbarPanel::OnSetSpeed(wxCommandEvent& event)
{
	double x, y;
	m_ctrl_xspeed->GetValue().ToDouble(&x);
	m_ctrl_yspeed->GetValue().ToDouble(&y);
	m_spr->SetSpeed(sm::vec2(x, y));
}

void ToolbarPanel::OnChangeType(wxCommandEvent& event)
{
	int idx = event.GetSelection();
	SetEditType(gum::MeshType(idx));
	m_stage->RecreateMesh();
}

}