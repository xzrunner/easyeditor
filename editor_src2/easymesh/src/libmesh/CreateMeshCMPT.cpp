#include "CreateMeshCMPT.h"
#include "CreateMeshOP.h"
#include "StagePanel.h"
#include "Mesh.h"
#include "FileIO.h"

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
	sizer->AddSpacer(15);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Copy..."));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CreateMeshCMPT::onCopy));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Clear"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CreateMeshCMPT::onClear));
		sizer->Add(btn);
	}
	return sizer;
}

void CreateMeshCMPT::onCopy(wxCommandEvent& event)
{
	std::string tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_mesh);
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		wxT("*_") + tag + wxT(".json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		FileIO::loadStruct(dlg.GetPath(), *m_stage->getMesh());
		m_editPanel->Refresh();
	}
}

void CreateMeshCMPT::onClear(wxCommandEvent& event)
{
	m_stage->getMesh()->clear();
	m_editPanel->Refresh();
	m_editOP->clear();
}

}