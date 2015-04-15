#include "SettingsDialog.h"

#include <easymap.h>

namespace emap
{

SettingDialog::SettingDialog(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Settings"))
{
	InitLayout();
}

void SettingDialog::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxHORIZONTAL);

	SettingCfg* cfg = SettingCfg::Instance();
	//
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("View"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "width"));

			m_width_ctrl = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(cfg->m_view_width), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_width_ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SettingDialog::OnViewSizeChanged));
			sz->Add(m_width_ctrl);

			sizer->Add(sz);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "height"));

			m_height_ctrl = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(cfg->m_view_height), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_height_ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SettingDialog::OnViewSizeChanged));
			sz->Add(m_height_ctrl);

			sizer->Add(sz);
		}
		top_sizer->Add(sizer);
	}
	//

	SetSizer(top_sizer);

	top_sizer->Layout();
	Refresh();
}

void SettingDialog::OnViewSizeChanged(wxCommandEvent& event)
{
	double width, height;
	m_width_ctrl->GetValue().ToDouble(&width);
	m_height_ctrl->GetValue().ToDouble(&height);

	SettingCfg* cfg = SettingCfg::Instance();
	cfg->m_view_width = width;
	cfg->m_view_height = height;
}

}