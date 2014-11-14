#include "CreateMeshCMPT.h"
#include "CreateMeshOP.h"
#include "CreateStripOP.h"
#include "StagePanel.h"
#include "FileIO.h"
#include "ShapeFactory.h"
#include "Shape.h"

namespace emesh
{

CreateMeshCMPT::CreateMeshCMPT(wxWindow* parent, const wxString& name,
					   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
{
	m_mesh_op = new CreateMeshOP(stage);
	m_strip_op= new CreateStripOP(stage);
	m_editOP = m_mesh_op;
}

CreateMeshCMPT::~CreateMeshCMPT()
{
	m_mesh_op->release();
	m_strip_op->release();
}

wxSizer* CreateMeshCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(15);
	{
		wxArrayString choices;
		choices.Add(wxT("mesh"));
		choices.Add(wxT("strip"));
		wxRadioBox* typeChoice = new wxRadioBox(this, wxID_ANY, wxT("Type"), 
			wxDefaultPosition, wxDefaultSize, choices, 2, wxRA_SPECIFY_COLS);
// 		typeChoice->SetSelection(1);
// 		ShapeFactory::Instance()->SetShapeType(ST_STRIP);
		typeChoice->SetSelection(0);
		ShapeFactory::Instance()->SetShapeType(ST_MESH);
 		Connect(typeChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
 			wxCommandEventHandler(CreateMeshCMPT::onChangeType));
		sizer->Add(typeChoice);

		sizer->AddSpacer(5);

		wxCheckBox* regionSet = new wxCheckBox(this, wxID_ANY, wxT("Ê¹ÓÃ±ß½ç"), 
			wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
		bool use_region = ShapeFactory::Instance()->IsUseRegion();
		regionSet->SetValue(use_region);
		Connect(regionSet->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(CreateMeshCMPT::onChangeUseRegion));
		sizer->Add(regionSet);
	}
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

void CreateMeshCMPT::onChangeType(wxCommandEvent& event)
{
	int idx = event.GetSelection();
	switch (idx)
	{
	case 0:
		ShapeFactory::Instance()->SetShapeType(ST_MESH);
		m_editOP = m_mesh_op;
		m_editPanel->setEditOP(m_editOP);
		m_stage->CreateShape();
		break;
	case 1:
		ShapeFactory::Instance()->SetShapeType(ST_STRIP);
		m_editOP = m_strip_op;
		m_editPanel->setEditOP(m_editOP);
		m_stage->CreateShape();
		break;
	}
}

void CreateMeshCMPT::onChangeUseRegion(wxCommandEvent& event)
{
	ShapeFactory::Instance()->SetUseRegion(event.IsChecked());
}

void CreateMeshCMPT::onCopy(wxCommandEvent& event)
{
// 	std::string tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_mesh);
// 	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
// 		wxT("*_") + tag + wxT(".json"), wxFD_OPEN);
// 	if (dlg.ShowModal() == wxID_OK)
// 	{
// 		FileIO::loadStruct(dlg.GetPath(), *m_stage->getMesh());
// 		m_editPanel->Refresh();
// 	}
}

void CreateMeshCMPT::onClear(wxCommandEvent& event)
{
	if (Shape* shape = m_stage->GetShape()) {
		shape->Clear();
	}
	m_stage->clearShapes();

	m_editPanel->Refresh();
	m_editOP->clear();
}

}