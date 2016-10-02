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
	m_network_op = new CreateNetworkOP(stage);
	m_strip_op= new CreateStripOP(stage);
	m_skeleton_op = new CreateSkeletonOP(stage);
	LoadEditOP(m_network_op);
}

CreateMeshCMPT::~CreateMeshCMPT()
{
	m_network_op->RemoveReference();
	m_strip_op->RemoveReference();
	m_skeleton_op->RemoveReference();
}

void CreateMeshCMPT::SetEditOP(gum::MeshType type)
{
	switch (type)
	{
	case gum::MESH_NETWORK:
		LoadEditOP(m_network_op);
		break;
	case gum::MESH_STRIP:
		LoadEditOP(m_strip_op);
		break;
	case gum::MESH_SKELETON:
		LoadEditOP(m_skeleton_op);
		break;
	}
}

wxSizer* CreateMeshCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(15);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Copy..."));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CreateMeshCMPT::OnCopy));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Clear"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CreateMeshCMPT::OnClear));
		sizer->Add(btn);
	}
	return sizer;
}

void CreateMeshCMPT::OnCopy(wxCommandEvent& event)
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

void CreateMeshCMPT::OnClear(wxCommandEvent& event)
{
	if (Mesh* mesh = m_stage->GetMesh()) {
		mesh->Clear();
	}
	ee::ClearShapeSJ::Instance()->Clear();

	m_editop->Clear();
}

}