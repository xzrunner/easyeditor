#include "CreateRegionCMPT.h"
#include "StagePanel.h"

namespace eterrain2d
{

CreateRegionCMPT::CreateRegionCMPT(wxWindow* parent, const wxString& name, StagePanel* stage)
	: libshape::NodeCaptureCMPT<libshape::EditPolylineOP<CreateRegionOP, d2d::SelectShapesOP> >(parent, name, stage, stage, NULL)
{
}

wxSizer* CreateRegionCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

 	wxSizer* psizer = libshape::NodeCaptureCMPT<libshape::EditPolylineOP<CreateRegionOP, d2d::SelectShapesOP> >::initLayout();
 	sizer->Add(psizer);
	sizer->AddSpacer(20);
	
	wxCheckBox* tris_edge = new wxCheckBox(this, wxID_ANY, wxT("triangle edge"));
	tris_edge->SetValue(d2d::Settings::bDisplayTrisEdge);
	Connect(tris_edge->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
		wxCommandEventHandler(CreateRegionCMPT::OnChangeDisplayTriangles));
	sizer->Add(tris_edge);

	return sizer;
}

void CreateRegionCMPT::OnChangeDisplayTriangles(wxCommandEvent& event)
{
	d2d::Settings::bDisplayTrisEdge = event.IsChecked();
	m_editPanel->Refresh();
}

}