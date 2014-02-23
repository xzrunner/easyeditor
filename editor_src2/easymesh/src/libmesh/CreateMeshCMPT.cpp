#include "CreateMeshCMPT.h"
#include "CreateMeshOP.h"
#include "StagePanel.h"

namespace emesh
{

CreateMeshCMPT::CreateMeshCMPT(wxWindow* parent, const wxString& name,
					   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
{
	m_editOP = new CreateMeshOP(stage);
}

wxSizer* CreateMeshCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	wxButton* btnClear = new wxButton(this, wxID_ANY, wxT("Clear"));
// 	Connect(btnLoad->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
// 		wxCommandEventHandler(CommonCMPT::onLoadAllFrameImages));
	sizer->Add(btnClear);
	return sizer;
}

}