#include "EditMeshCMPT.h"
#include "EditNetworkOP.h"
#include "EditSkeletonOP.h"
#include "StagePanel.h"
#include "Mesh.h"

#include <ee/DrawRectangleOP.h>
#include <ee/panel_msg.h>

namespace emesh
{

EditMeshCMPT::EditMeshCMPT(wxWindow* parent, const std::string& name,
						   StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_network_op = new EditNetworkOP(stage);
	m_skeleton_op = new EditSkeletonOP(stage);
	LoadEditOP(m_network_op);
}

EditMeshCMPT::~EditMeshCMPT()
{
	m_network_op->RemoveReference();
	m_skeleton_op->RemoveReference();
}

void EditMeshCMPT::SetEditOP(gum::MeshType type)
{
	switch (type)
	{
	case gum::MESH_NETWORK: case gum::MESH_STRIP:
		LoadEditOP(m_network_op);
		break;
	case gum::MESH_SKELETON:
		LoadEditOP(m_skeleton_op);
		break;
	}
}

wxSizer* EditMeshCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	wxButton* btnReset = new wxButton(this, wxID_ANY, wxT("Reset"));
 	Connect(btnReset->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
 		wxCommandEventHandler(EditMeshCMPT::OnReset));
	sizer->Add(btnReset);
	return sizer;
}

void EditMeshCMPT::OnReset(wxCommandEvent& event)
{
	if (Mesh* mesh = m_stage->GetMesh()) {
		mesh->Reset();
	}
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
	ee::SetWndDirtySJ::Instance()->SetDirty();
	m_editop->Clear();
}

}