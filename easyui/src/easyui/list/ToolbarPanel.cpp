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

void ToolbarPanel::EnableHori(bool enable)
{
	m_hori_check->SetValue(enable);
}

void ToolbarPanel::EnableVert(bool enable)
{
	m_vert_check->SetValue(enable);
}

void ToolbarPanel::EnableReverseOrder(bool enable)
{
	m_reverse_check->SetValue(enable);
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
		sm::vec2 sz = m_stage_panel->GetList().GetClipbox().Size();
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "width"), 0, wxLEFT | wxRIGHT, 5);

			m_width_text = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(sz.x),
				wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
			Connect(m_width_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			csizer->Add(m_width_text);

			sizer->Add(csizer);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
			csizer->Add(new wxStaticText(this, wxID_ANY, "height"), 0, wxLEFT | wxRIGHT, 5);

			m_height_text = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(sz.y),
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
	sizer->AddSpacer(20);
	{
		m_reverse_check = new wxCheckBox(this, wxID_ANY, wxT("reverse order"));
		Connect(m_reverse_check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnReverseOrder));
		sizer->Add(m_reverse_check);		
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

void ToolbarPanel::OnChangeHori(wxCommandEvent& event)
{
	m_stage_panel->GetList().EnableHori(event.IsChecked());
}

void ToolbarPanel::OnChangeVert(wxCommandEvent& event)
{
	m_stage_panel->GetList().EnableVert(event.IsChecked());
}

void ToolbarPanel::OnReverseOrder(wxCommandEvent& event)
{
	m_stage_panel->GetList().EnableReverseOrder(event.IsChecked());
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
}