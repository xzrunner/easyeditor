#include "EditMeshCMPT.h"
#include "EditMeshOP.h"
#include "StagePanel.h"

namespace emesh
{

EditMeshCMPT::EditMeshCMPT(wxWindow* parent, const wxString& name,
					   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
{
	m_editOP = new EditMeshOP(stage);
}

wxSizer* EditMeshCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	wxButton* btnReset = new wxButton(this, wxID_ANY, wxT("Reset"));
	// 	Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
	// 		wxCommandEventHandler(CommonCMPT::onLoadAllFrameImages));
	sizer->Add(btnReset);
	return sizer;
}

}