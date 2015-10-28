#include "ToolBarPanel.h"
#include "UIList.h"
#include "StagePanel.h"

namespace eui
{
namespace list
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage_panel)
	: d2d::ToolbarPanel(parent, NULL)
	, m_stage_panel(stage_panel)
{
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "filling");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnItemFilling));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "clear");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnItemClear));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("hori"));
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnChangeHori));
		sizer->Add(check);
	}
	sizer->AddSpacer(5);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("vert"));
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnChangeVert));
		sizer->Add(check);
	}
	return sizer;
}

void ToolbarPanel::OnItemFilling(wxCommandEvent& event)
{
	bool succ = m_stage_panel->GetList().Filling();
	if (succ) {
		m_stage_panel->SetCanvasDirty();
	}
}

void ToolbarPanel::OnItemClear(wxCommandEvent& event)
{
	m_stage_panel->GetList().ClearExceptBase();
	m_stage_panel->SetCanvasDirty();
}

void ToolbarPanel::OnChangeHori(wxCommandEvent& event)
{
	m_stage_panel->GetList().ChangeHori();
}

void ToolbarPanel::OnChangeVert(wxCommandEvent& event)
{
	m_stage_panel->GetList().ChangeVert();
}

}
}