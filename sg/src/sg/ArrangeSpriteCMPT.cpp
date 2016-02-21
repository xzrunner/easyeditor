#include "ArrangeSpriteCMPT.h"
#include "ArrangeSpriteOP.h"
#include "StagePanel.h"

namespace sg
{

ArrangeSpriteCMPT::ArrangeSpriteCMPT(wxWindow* parent, const std::string& name,
									 ee::PropertySettingPanel* property, 
									 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage_panel(stage)
{
	m_editop = new ArrangeSpriteOP(property, stage);
}

wxSizer* ArrangeSpriteCMPT::InitLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("level"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Up"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ArrangeSpriteCMPT::onUpLevelPress));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Down"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ArrangeSpriteCMPT::onDownLevelPress));
		sizer->Add(btn);
	}
	return sizer;
}

void ArrangeSpriteCMPT::onUpLevelPress(wxCommandEvent& event)
{
	m_stage_panel->ChangeSelectedSpritesLevel(true);
}

void ArrangeSpriteCMPT::onDownLevelPress(wxCommandEvent& event)
{
	m_stage_panel->ChangeSelectedSpritesLevel(false);	
}

}