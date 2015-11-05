#include "ToolbarPanel.h"
#include "StagePanel.h"

namespace eui
{
namespace window
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
		m_width_text = new wxTextCtrl(this, wxID_ANY, "width");
		Connect(m_width_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(ToolbarPanel::OnChangeSize));
		sizer->Add(m_width_text);
	}
	sizer->AddSpacer(5);
	{
		m_height_text = new wxTextCtrl(this, wxID_ANY, "height");
		Connect(m_height_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(ToolbarPanel::OnChangeSize));
		sizer->Add(m_height_text);
	}
	return sizer;
}

void ToolbarPanel::OnChangeSize(wxCommandEvent& event)
{
	double width, height;
	m_width_text->GetValue().ToDouble(&width);
	m_height_text->GetValue().ToDouble(&height);
	m_stage_panel->SetViewSize(width, height);
}

}
}

