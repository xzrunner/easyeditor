#include "CreateMeshCMPT.h"
#include "CreatePointsMeshOP.h"
#include "CreateStripMeshOP.h"
#include "CreateSkinMeshOP.h"
#include "StagePanel.h"
#include "FileIO.h"
#include "MeshFactory.h"
#include "Mesh.h"

#include <ee/shape_msg.h>

#include <polymesh/MeshType.h>

namespace emesh
{

CreateMeshCMPT::CreateMeshCMPT(wxWindow* parent, const std::string& name,
									 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_network_op = std::make_shared<CreatePointsMeshOP>(stage);
	m_strip_op = std::make_shared<CreateStripOP>(stage);
	m_skeleton_op = std::make_shared<CreateSkeletonOP>(stage);
	LoadEditOP(m_network_op);
}

void CreateMeshCMPT::SetEditOP(int pm_mesh_type)
{
	switch (pm_mesh_type)
	{
	case pm::MESH_POINTS:
		LoadEditOP(m_network_op);
		break;
	case pm::MESH_STRIP:
		LoadEditOP(m_strip_op);
		break;
	case pm::MESH_SKIN:
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
// 	std::string tag = ee::SymbolFile::Instance()->Tag(ee::FileType::e_mesh);
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