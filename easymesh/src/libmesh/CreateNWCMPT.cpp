#include "CreateNWCMPT.h"
#include "CreateNWOP.h"
#include "CreateStripOP.h"
#include "StagePanel.h"
#include "FileIO.h"
#include "MeshFactory.h"
#include "Mesh.h"

#include <ee/shape_msg.h>

namespace emesh
{

CreateNWCMPT::CreateNWCMPT(wxWindow* parent, const std::string& name,
							   StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_mesh_op = new CreateNWOP(stage);
	m_strip_op= new CreateStripOP(stage);
	LoadEditOP(m_mesh_op);
}

CreateNWCMPT::~CreateNWCMPT()
{
	m_mesh_op->RemoveReference();
	m_strip_op->RemoveReference();
}

wxSizer* CreateNWCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(15);
	{
		wxArrayString choices;
		choices.Add(wxT("network"));
		choices.Add(wxT("strip"));
		wxRadioBox* typeChoice = new wxRadioBox(this, wxID_ANY, wxT("Type"), 
			wxDefaultPosition, wxDefaultSize, choices, 2, wxRA_SPECIFY_ROWS);
// 		typeChoice->SetSelection(1);
// 		MeshFactory::Instance()->SetShapeType(ST_STRIP);
		typeChoice->SetSelection(0);
		MeshFactory::Instance()->SetShapeType(ST_NETWORK);
 		Connect(typeChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
 			wxCommandEventHandler(CreateNWCMPT::onChangeType));
		sizer->Add(typeChoice);
	}
	sizer->AddSpacer(15);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Copy..."));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CreateNWCMPT::onCopy));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Clear"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(CreateNWCMPT::onClear));
		sizer->Add(btn);
	}
	return sizer;
}

void CreateNWCMPT::onChangeType(wxCommandEvent& event)
{
	int idx = event.GetSelection();
	switch (idx)
	{
	case 0:
		MeshFactory::Instance()->SetShapeType(ST_NETWORK);
		LoadEditOP(m_mesh_op);
		m_stage->RecreateMesh();
		break;
	case 1:
		MeshFactory::Instance()->SetShapeType(ST_STRIP);
		LoadEditOP(m_strip_op);
		m_stage->RecreateMesh();
		break;
	}
}

void CreateNWCMPT::onCopy(wxCommandEvent& event)
{
// 	std::string tag = ee::FileType::GetTag(ee::FileType::e_mesh);
// 	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
// 		wxT("*_") + tag + wxT(".json"), wxFD_OPEN);
// 	if (dlg.ShowModal() == wxID_OK)
// 	{
// 		FileIO::loadStruct(dlg.GetPath(), *m_stage->getMesh());
// 		m_stage->Refresh();
// 	}
}

void CreateNWCMPT::onClear(wxCommandEvent& event)
{
	if (Mesh* mesh = m_stage->GetMesh()) {
		mesh->Clear();
	}
	ee::ClearShapeSJ::Instance()->Clear();

	m_editop->Clear();
}

}