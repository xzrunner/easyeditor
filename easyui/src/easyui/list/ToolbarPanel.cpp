#include "ToolBarPanel.h"
#include "UIList.h"
#include "StagePanel.h"

#include <ee/StringHelper.h>
#include <ee/panel_msg.h>

namespace eui
{
namespace list
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage_panel)
	: ee::ToolbarPanel(parent, NULL)
	, m_stage_panel(stage_panel)
{
	SetSizer(InitLayout());	
}

void ToolbarPanel::EnableReverseOrder(bool enable)
{
	m_reverse_check->SetValue(enable);
}

void ToolbarPanel::EnableColumnOrder(bool enable)
{
	m_column_check->SetValue(enable);
}

void ToolbarPanel::SetSizeText(int width, int height)
{
	m_width_text->SetValue(ee::StringHelper::ToString(width));
	m_height_text->SetValue(ee::StringHelper::ToString(height));
}

wxSizer* ToolbarPanel::InitLayout()
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
		wxSizer* hori_sizer = new wxBoxSizer(wxHORIZONTAL);
		sm::vec2 sz = m_stage_panel->GetList().GetClipbox().Size();
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "width"), 0, wxLEFT | wxRIGHT, 5);

			m_width_text = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(sz.x),
				wxDefaultPosition, wxSize(80, -1), wxTE_PROCESS_ENTER);
			Connect(m_width_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			csizer->Add(m_width_text);

			hori_sizer->Add(csizer);
		}
		hori_sizer->AddSpacer(10);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "height"), 0, wxLEFT | wxRIGHT, 5);

			m_height_text = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(sz.y),
				wxDefaultPosition, wxSize(80, -1), wxTE_PROCESS_ENTER);
			Connect(m_height_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			csizer->Add(m_height_text);

			hori_sizer->Add(csizer);
		}
		sizer->Add(hori_sizer);
	}
	sizer->AddSpacer(20);
	{
		const int size = 2;
		wxString choices[size];
		choices[0] = "hori";
		choices[1] = "vert";
		m_check_dir = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, size, choices);
		m_check_dir->SetSelection(1);
		Connect(m_check_dir->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
			wxCommandEventHandler(ToolbarPanel::OnChangeCheck));
		sizer->Add(m_check_dir);
	}
	sizer->AddSpacer(20);
	{
		wxSizer* hori_sizer = new wxBoxSizer(wxHORIZONTAL);
		{
			m_reverse_check = new wxCheckBox(this, wxID_ANY, wxT("reverse order"));
			Connect(m_reverse_check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(ToolbarPanel::OnReverseOrder));
			hori_sizer->Add(m_reverse_check);		
		}
		hori_sizer->AddSpacer(10);
		{
			m_column_check = new wxCheckBox(this, wxID_ANY, wxT("column major order"));
			Connect(m_column_check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(ToolbarPanel::OnColumnOrder));
			hori_sizer->Add(m_column_check);		
		}
		sizer->Add(hori_sizer);
	}
	return sizer;
}

void ToolbarPanel::OnItemFilling(wxCommandEvent& event)
{
	bool succ = m_stage_panel->GetList().ReFilling();
	if (succ) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void ToolbarPanel::OnChangeSize(wxCommandEvent& event)
{
	int hw = ee::StringHelper::FromString<float>(m_width_text->GetValue().ToStdString()) * 0.5f;
	int hh = ee::StringHelper::FromString<float>(m_height_text->GetValue().ToStdString()) * 0.5f;

	sm::rect& r = m_stage_panel->GetList().GetClipbox();
	r.xmin = -hw;
	r.xmax = hw;
	r.ymin = -hh;
	r.ymax = hh;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ToolbarPanel::OnChangeCheck(wxCommandEvent& event)
{
	UIList& list = m_stage_panel->GetList();
	if (event.GetSelection() == 0) {
		list.EnableHori(true);
		list.EnableVert(false);
	} else {
		list.EnableHori(false);
		list.EnableVert(true);
	}
}

void ToolbarPanel::OnReverseOrder(wxCommandEvent& event)
{
	m_stage_panel->GetList().EnableReverseOrder(event.IsChecked());
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ToolbarPanel::OnColumnOrder(wxCommandEvent& event)
{
	m_stage_panel->GetList().EnableColumnOrder(event.IsChecked());
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
}