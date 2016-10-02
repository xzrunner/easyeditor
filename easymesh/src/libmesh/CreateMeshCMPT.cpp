#include "CreateMeshCMPT.h"
#include "CreateNetworkOP.h"
#include "CreateStripOP.h"
#include "CreateSkeletonOP.h"
#include "StagePanel.h"
#include "FileIO.h"
#include "MeshFactory.h"
#include "Mesh.h"

#include <ee/shape_msg.h>

namespace emesh
{

CreateMeshCMPT::CreateMeshCMPT(wxWindow* parent, const std::string& name,
									 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_mesh_op = new CreateNetworkOP(stage);
	m_strip_op= new CreateStripOP(stage);
	m_skeleton_op = new CreateSkeletonOP(stage);
	LoadEditOP(m_mesh_op);
}

CreateMeshCMPT::~CreateMeshCMPT()
{
	m_mesh_op->RemoveReference();
	m_strip_op->RemoveReference();
	m_skeleton_op->RemoveReference();
}

wxSizer* CreateMeshCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(15);
	{
		wxArrayString choices;
		choices.Add(wxT("network"));
		choices.Add(wxT("strip"));
		choices.Add(wxT("skeleton"));
		wxRadioBox* typeChoice = new wxRadioBox(this, wxID_ANY, wxT("Type"), 
			wxDefaultPosition, wxDefaultSize, choices, 3, wxRA_SPECIFY_ROWS);
// 		typeChoice->SetSelection(1);
// 		MeshFactory::Instance()->SetShapeType(ST_STRIP);
		typeChoice->SetSelection(0);
		MeshFactory::Instance()->SetShapeType(ST_NETWORK);
 		Connect(typeChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
 			wxCommandEventHandler(CreateMeshCMPT::onChangeType));
		sizer->Add(typeChoice);
	}
	sizer->AddSpacer(15);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Copy..."));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CreateMeshCMPT::onCopy));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Clear"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CreateMeshCMPT::onClear));
		sizer->Add(btn);
	}
	return sizer;
}

void CreateMeshCMPT::onChangeType(wxCommandEvent& event)
{
	int idx = event.GetSelection();
	switch (idx)
	{
	case 0:
		MeshFactory::Instance()->SetShapeType(ST_NETWORK);
		LoadEditOP(m_mesh_op);
		m_stage->RecreateMesh();
		break;
	case 1:
		MeshFactory::Instance()->SetShapeType(ST_STRIP);
		LoadEditOP(m_strip_op);
		m_stage->RecreateMesh();
		break;
	case 2:
		MeshFactory::Instance()->SetShapeType(ST_SKELETON);
		LoadEditOP(m_skeleton_op);
		m_stage->RecreateMesh();
		break;
	}
}

void CreateMeshCMPT::onCopy(wxCommandEvent& event)
{
// 	std::string tag = ee::FileType::GetTag(ee::FileType::e_mesh);
// 	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
// 		wxT("*_") + tag + wxT(".json"), wxFD_OPEN);
// 	if (dlg.ShowModal() == wxID_OK)
// 	{
// 		FileIO::loadStruct(dlg.GetPath(), *m_stage->getMesh());
// 		m_stage->Refresh();
// 	}
}

void CreateMeshCMPT::onClear(wxCommandEvent& event)
{
	if (Mesh* mesh = m_stage->GetMesh()) {
		mesh->Clear();
	}
	ee::ClearShapeSJ::Instance()->Clear();

	m_editop->Clear();
}

}