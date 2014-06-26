#include "EditUVCMPT.h"
#include "StagePanel.h"
#include "EditUVOP.h"
#include "Mesh.h"

namespace emesh
{

EditUVCMPT::EditUVCMPT(wxWindow* parent, const wxString& name, 
					   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
{
	m_editOP = new EditUVOP(stage);
}

wxSizer* EditUVCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	{
		wxButton* btnMove = new wxButton(this, wxID_ANY, wxT("Move"));
		Connect(btnMove->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(EditUVCMPT::onMove));
		sizer->Add(btnMove);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btnReset = new wxButton(this, wxID_ANY, wxT("Reset"));
		Connect(btnReset->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(EditUVCMPT::onReset));
		sizer->Add(btnReset);
	}
	return sizer;
}

void EditUVCMPT::onMove(wxCommandEvent& event)
{
	m_stage->getShape()->ResetUVOffset(0, -0.15f);
	m_editPanel->Refresh();
}

void EditUVCMPT::onReset(wxCommandEvent& event)
{
	m_stage->getShape()->Reset();
	m_editPanel->Refresh();
	m_editOP->clear();
}

}