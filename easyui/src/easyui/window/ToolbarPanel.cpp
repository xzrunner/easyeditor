#include "ToolbarPanel.h"
#include "StagePanel.h"

#include <ee/StringHelper.h>
#include <ee/panel_msg.h>

#include <easyui.h>

namespace eui
{
namespace window
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage_panel)
	: ee::ToolbarPanel(parent, NULL)
	, m_stage_panel(stage_panel)
{
	SetSizer(InitLayout());	
	RegistSubject(ChangeWindowViewSizeSJ::Instance());
}

std::string ToolbarPanel::GetWindowName() const
{
	return m_name_text->GetValue().ToStdString();
}

void ToolbarPanel::SetWindowName(const std::string& name)
{
	m_name_text->SetValue(name);
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(10);
	top_sizer->Add(InitNameLayout());
	top_sizer->AddSpacer(10);
	top_sizer->Add(InitViewLayout());
	top_sizer->AddSpacer(10);
	top_sizer->Add(InitCrossLayout());
	return top_sizer;
}

void ToolbarPanel::OnNotify(int sj_id, void* ud)
{
	if (sj_id == MSG_CHANGE_WINDOW_VIEW_SIZE) 
	{
		ChangeWindowViewSizeSJ::Params* p = (ChangeWindowViewSizeSJ::Params*)ud;
		m_width_text->SetValue(ee::StringHelper::ToString(p->width));
		m_height_text->SetValue(ee::StringHelper::ToString(p->height));
	}
}

wxSizer* ToolbarPanel::InitNameLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Name"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	m_name_text = new wxTextCtrl(this, wxID_ANY);
	sizer->Add(m_name_text);

	return sizer;
}

wxSizer* ToolbarPanel::InitViewLayout()
{
	int width, height;
	window::QueryWindowViewSizeSJ::Instance()->Query(width, height);

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("View"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "width"));

		m_width_text = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(width), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
		Connect(m_width_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
		sz->Add(m_width_text);

		sizer->Add(sz);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "height"));

		m_height_text = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(height), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
		Connect(m_height_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
		sz->Add(m_height_text);

		sizer->Add(sz);
	}
	return sizer;
}

// todo copy from eanim::NormalCMPT
wxSizer* ToolbarPanel::InitCrossLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("¸¨ÖúÏß"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

	wxButton* btn_add = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(btn_add, 0, wxLEFT | wxRIGHT, 5);
	Connect(btn_add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(ToolbarPanel::OnAddCross));
	wxButton* btn_del = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(btn_del, 0, wxLEFT | wxRIGHT, 5);
	Connect(btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(ToolbarPanel::OnDelCross));

	return sizer;
}

void ToolbarPanel::OnChangeSize(wxCommandEvent& event)
{
	double width, height;
	m_width_text->GetValue().ToDouble(&width);
	m_height_text->GetValue().ToDouble(&height);
	m_stage_panel->SetViewSize(width, height);
}

void ToolbarPanel::OnAddCross(wxCommandEvent& event)
{
	m_stage_panel->GetCrossGuides().Add();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}
	
void ToolbarPanel::OnDelCross(wxCommandEvent& event)
{
	m_stage_panel->GetCrossGuides().Remove();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
}

