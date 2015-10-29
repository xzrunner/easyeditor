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

void ToolbarPanel::EnableHori(bool enable)
{
	m_hori_check->SetValue(enable);
}

void ToolbarPanel::EnableVert(bool enable)
{
	m_vert_check->SetValue(enable);
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
		m_hori_check = new wxCheckBox(this, wxID_ANY, wxT("hori"));
		Connect(m_hori_check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnChangeHori));
		sizer->Add(m_hori_check);
	}
	sizer->AddSpacer(5);
	{
		m_vert_check = new wxCheckBox(this, wxID_ANY, wxT("vert"));
		Connect(m_vert_check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnChangeVert));
		sizer->Add(m_vert_check);
	}
	return sizer;
}

void ToolbarPanel::OnItemFilling(wxCommandEvent& event)
{
	bool succ = m_stage_panel->GetList().ReFilling();
	if (succ) {
		m_stage_panel->SetCanvasDirty();
	}
}

void ToolbarPanel::OnChangeHori(wxCommandEvent& event)
{
	m_stage_panel->GetList().EnableHori(event.IsChecked());
}

void ToolbarPanel::OnChangeVert(wxCommandEvent& event)
{
	m_stage_panel->GetList().EnableVert(event.IsChecked());
}

}
}