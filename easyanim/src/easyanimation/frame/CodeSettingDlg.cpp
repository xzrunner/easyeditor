#include "CodeSettingDlg.h"

namespace eanim
{

CodeSettingDlg::CodeSettingDlg(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Setting"))
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	{
		wxSizer* packerSizer = new wxBoxSizer(wxHORIZONTAL);
		packerSizer->Add(new wxStaticText(this, wxID_ANY, "Texture Packer File: "), 0, wxLEFT | wxRIGHT, 5);
		
		m_tp_path = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(225, -1), wxTE_READONLY);
//		m_tp_path->SetValue(ctrl->packer);
		packerSizer->Add(m_tp_path, 0, wxLEFT | wxRIGHT, 5);

		wxButton* btn = new wxButton(this, wxID_ANY, "...", wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CodeSettingDlg::OnSelectTPFile));
		packerSizer->Add(btn, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(packerSizer);
	}
	SetSizer(sizer);

	sizer->Layout();
	Refresh(true);
}

void CodeSettingDlg::OnSelectTPFile(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Texture Packer file"), wxEmptyString, wxEmptyString, 
		wxT("*_packer.json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
// 		m_ctrl->packer = dlg.GetPath();
// 		m_tp_path->SetValue(m_ctrl->packer);
	}
}
} // eanim