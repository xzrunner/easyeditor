#include "EditNWCMPT.h"
#include "EditNWOP.h"
#include "StagePanel.h"
#include "Mesh.h"

#include <ee/DrawRectangleOP.h>
#include <ee/panel_msg.h>

namespace emesh
{

EditNWCMPT::EditNWCMPT(wxWindow* parent, const std::string& name,
						   StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_editop = new EditNWOP(stage);
}

wxSizer* EditNWCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	wxButton* btnReset = new wxButton(this, wxID_ANY, wxT("Reset"));
 	Connect(btnReset->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
 		wxCommandEventHandler(EditNWCMPT::OnReset));
	sizer->Add(btnReset);
	return sizer;
}

void EditNWCMPT::OnReset(wxCommandEvent& event)
{
	if (Mesh* mesh = m_stage->GetMesh()) {
		mesh->Reset();
	}
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
	ee::SetWndDirtySJ::Instance()->SetDirty();
	m_editop->Clear();
}

}