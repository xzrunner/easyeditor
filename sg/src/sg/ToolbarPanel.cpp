#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "ArrangeSpriteCMPT.h"
#include "ResourceMgr.h"

static const int ROW = 44;
static const int COL = 44;
static const int EDGE = 48;

namespace sg
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library, 
						   d2d::PropertySettingPanel* property, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
	, m_stage(stage)
{
	addChild(new d2d::UniversalCMPT(this, wxT("paste"), stage, 
		new d2d::PasteSymbolOP(stage, stage, library)));
	addChild(new d2d::PasteSpriteCMPT(this, wxT("batch"), stage, stage, property));
	addChild(new ArrangeSpriteCMPT(this, wxT("arrange"), stage, property));

	SetSizer(initLayout());	

	stage->setLayoutInfo(ROW, COL, EDGE);
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(10);
	initOptSetting(sizer);
	sizer->AddSpacer(20);
	sizer->Add(initChildrenLayout());
	return sizer;
}

void ToolbarPanel::initOptSetting(wxSizer* sizer)
{
	wxChoice* choice = new wxChoice(this, wxID_ANY);
	for (int i = 1; i <= 10; ++i) {
		std::string str = "Townhall " + wxString::FromDouble(i);
		choice->Append(str);	
	}
	sizer->Add(choice, 0, wxALIGN_LEFT);

	Connect(choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxCommandEventHandler(ToolbarPanel::onLevelChanged));
	choice->SetSelection(0);
}

void ToolbarPanel::initSizeSetting(wxBoxSizer* topSizer)
{
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("row:")));

		m_row = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 2, 100, ROW);
		Connect(m_row->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_row);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("col:")));

		m_col = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 2, 100, ROW);
		Connect(m_col->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_col);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(20);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("length:")));

		m_edge = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 16, 128, EDGE);
		Connect(m_edge->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_edge);

		topSizer->Add(sizer);
	}
	onParamsChanged(wxSpinEvent());
	topSizer->AddSpacer(20);
}

void ToolbarPanel::onParamsChanged(wxSpinEvent& event)
{
	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	stage->setLayoutInfo(m_row->GetValue(), 
		m_col->GetValue(), m_edge->GetValue());
	stage->updateAllSpritesLocation();
	m_editPanel->Refresh();
}

void ToolbarPanel::onLevelChanged(wxCommandEvent& event)
{
	m_stage->setLevel(event.GetSelection() + 1);
	m_stage->getResourceMgr()->initLibraryFromBuildings();
}

}