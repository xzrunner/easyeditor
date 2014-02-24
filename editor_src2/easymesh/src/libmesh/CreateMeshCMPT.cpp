#include "CreateMeshCMPT.h"
#include "CreateMeshOP.h"
#include "StagePanel.h"
#include "Mesh.h"

namespace emesh
{

CreateMeshCMPT::CreateMeshCMPT(wxWindow* parent, const wxString& name,
					   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
{
	m_editOP = new CreateMeshOP(stage);
}

wxSizer* CreateMeshCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	wxButton* btnClear = new wxButton(this, wxID_ANY, wxT("Clear"));
 	Connect(btnClear->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
 		wxCommandEventHandler(CreateMeshCMPT::onClear));
	sizer->Add(btnClear);
	return sizer;
}

void CreateMeshCMPT::onClear(wxCommandEvent& event)
{
	m_stage->getMesh()->clear();
	m_editPanel->Refresh();
	m_editOP->clear();
}

}