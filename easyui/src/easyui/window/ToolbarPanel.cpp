#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "message_id.h"
#include "QueryWindowViewSizeSJ.h"
#include "ChangeWindowViewSizeSJ.h"

namespace eui
{
namespace window
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage_panel)
	: d2d::ToolbarPanel(parent, NULL)
	, m_stage_panel(stage_panel)
{
	SetSizer(initLayout());	
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

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->AddSpacer(10);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Name"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_name_text = new wxTextCtrl(this, wxID_ANY);
		sizer->Add(m_name_text);

		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	{
		int width, height;
		window::QueryWindowViewSizeSJ::Instance()->Query(width, height);

		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("View"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "width"));

			m_width_text = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(width), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_width_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			sz->Add(m_width_text);

			sizer->Add(sz);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "height"));

			m_height_text = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(height), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_height_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ToolbarPanel::OnChangeSize));
			sz->Add(m_height_text);

			sizer->Add(sz);
		}
		top_sizer->Add(sizer);
	}
	return top_sizer;
}

void ToolbarPanel::OnNotify(int sj_id, void* ud)
{
	if (sj_id == MSG_CHANGE_WINDOW_VIEW_SIZE) 
	{
		ChangeWindowViewSizeSJ::Params* p = (ChangeWindowViewSizeSJ::Params*)ud;
		m_width_text->SetValue(d2d::StringTools::ToString(p->width));
		m_height_text->SetValue(d2d::StringTools::ToString(p->height));
	}
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

