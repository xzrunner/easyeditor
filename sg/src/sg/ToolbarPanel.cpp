#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "ArrangeSpriteCMPT.h"

namespace sg
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library, 
						   d2d::PropertySettingPanel* property, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new d2d::UniversalCMPT(this, wxT("paste"), stage, 
		new d2d::PasteSymbolOP(stage, stage, library)));
	addChild(new d2d::PasteSpriteCMPT(this, wxT("batch"), stage, stage, property));
	//addChild(new d2d::UniversalCMPT(this, wxT("arrange"), stage, 
	//	new d2d::ArrangeSpriteFixOP(stage, stage, property)));

	addChild(new ArrangeSpriteCMPT(this, wxT("arrange"), stage, property));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Settings"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("row:")));

		m_row = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 2, 100, 44);
		Connect(m_row->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_row);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("col:")));

		m_col = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 2, 100, 44);
		Connect(m_col->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_col);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(20);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("length:")));

		m_edge = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 16, 128, 48);
		Connect(m_edge->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_edge);

		topSizer->Add(sizer);
	}
	onParamsChanged(wxSpinEvent());
	topSizer->AddSpacer(20);
	topSizer->Add(initChildrenLayout());

	return topSizer;
}

void ToolbarPanel::onParamsChanged(wxSpinEvent& event)
{
	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	stage->setLayoutInfo(m_row->GetValue(), 
		m_col->GetValue(), m_edge->GetValue());
	stage->updateAllSpritesLocation();
	m_editPanel->Refresh();
}

}