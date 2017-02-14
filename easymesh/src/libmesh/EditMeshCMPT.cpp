#include "EditMeshCMPT.h"
#include "EditPointsMeshOP.h"
#include "EditSkeletonOP.h"
#include "StagePanel.h"
#include "Mesh.h"

#include <ee/DrawRectangleOP.h>
#include <ee/panel_msg.h>

#include <polymesh/MeshType.h>

namespace emesh
{

EditMeshCMPT::EditMeshCMPT(wxWindow* parent, const std::string& name,
						   StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_points_op = new EditPointsMeshOP(stage);
	m_skin_op = new EditSkeletonOP(stage);
	LoadEditOP(m_points_op);
}

EditMeshCMPT::~EditMeshCMPT()
{
	m_points_op->RemoveReference();
	m_skin_op->RemoveReference();
}

void EditMeshCMPT::SetEditOP(int pm_mesh_type)
{
	switch (pm_mesh_type)
	{
	case pm::MESH_POINTS: case pm::MESH_STRIP:
		LoadEditOP(m_points_op);
		break;
	case pm::MESH_SKIN:
		LoadEditOP(m_skin_op);
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