#include "ArrangeSpriteCMPT.h"
#include "ArrangeSpriteOP.h"
#include "StagePanel.h"

namespace sg
{

ArrangeSpriteCMPT::ArrangeSpriteCMPT(wxWindow* parent, const wxString& name,
									 d2d::PropertySettingPanel* property, 
									 d2d::ViewPanelMgr* view_panel_mgr,
									 StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage->GetStageImpl())
	, m_stage_panel(stage)
{
	m_editOP = new ArrangeSpriteOP(property, view_panel_mgr, stage);
}

wxSizer* ArrangeSpriteCMPT::initLayout()
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