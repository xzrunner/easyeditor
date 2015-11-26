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
	sizer->AddSpacer(20);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "filling");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnItemFilling));
		sizer->Add(btn);
	}
	sizer->AddSpacer(20);
	{
		const d2d::Rect& r = m_stage_panel->GetList().GetClipbox();
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "width"), 0, wxLEFT | wxRIGHT, 5);

			m_width_text = new wxTextCtrl(this, wxID_ANY, d2d::StringTools::ToString(r.xLength()),
				wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
			Connect(m_width_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			csizer->Add(m_width_text);

			sizer->Add(csizer);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "height"), 0, wxLEFT | wxRIGHT, 5);

			m_height_text = new wxTextCtrl(this, wxID_ANY, d2d::StringTools::ToString(r.yLength()),
				wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
			Connect(m_height_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			csizer->Add(m_height_text);

			sizer->Add(csizer);
		}
	}
	sizer->AddSpacer(20);
	{
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
	}
	return sizer;
}

void ToolbarPanel::OnItemFilling(wxCommandEvent& event)
{
	bool succ = m_stage_panel->GetList().ReFilling();
	if (succ) {
		m_stage_panel->d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void ToolbarPanel::OnChangeSize(wxCommandEvent& event)
{
	int hw = d2d::StringTools::FromString<float>(m_width_text->GetValue().ToStdString()) * 0.5f;
	int hh = d2d::StringTools::FromString<float>(m_height_text->GetValue().ToStdString()) * 0.5f;

	d2d::Rect& r = m_stage_panel->GetList().GetClipbox();
	r.xMin = -hw;
	r.xMax = hw;
	r.yMin = -hh;
	r.yMax = hh;

	m_stage_panel->d2d::SetCanvasDirtySJ::Instance()->SetDirty();
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