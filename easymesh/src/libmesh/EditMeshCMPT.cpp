#include "EditMeshCMPT.h"
#include "EditMeshOP.h"
#include "StagePanel.h"
#include "EditShape.h"

namespace emesh
{

EditMeshCMPT::EditMeshCMPT(wxWindow* parent, const wxString& name,
						   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
{
	m_editOP = new EditMeshOP(stage);
}

wxSizer* EditMeshCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	wxButton* btnReset = new wxButton(this, wxID_ANY, wxT("Reset"));
 	Connect(btnReset->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
 		wxCommandEventHandler(EditMeshCMPT::onReset));
	sizer->Add(btnReset);
	return sizer;
}

void EditMeshCMPT::onReset(wxCommandEvent& event)
{
	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape())) {
		shape->Reset();
	}
	m_stage->SetCanvasDirty();
	m_editOP->Clear();
}

}