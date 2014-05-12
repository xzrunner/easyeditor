
#include "SettingOutputDlg.h"

using namespace ebuilder;

SettingOutputDlg::SettingOutputDlg(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("output"))
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxArrayString choices;
		choices.Add(wxT("love2d"));
		choices.Add(wxT("libgdx"));
		choices.Add(wxT("cocos2d-x"));
		choices.Add(wxT("easy2d"));
//		choices.Add(wxT("unity"));

		m_typeChoice = new wxRadioBox(this, wxID_ANY, wxT("type"), 
			wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
		m_typeChoice->SetSelection(Settings::code);
		sizer->Add(m_typeChoice, 0, wxCENTRE | wxALL, 20);
	}
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
		btnSizer->Add(new wxButton(this, wxID_OK), 0, wxRIGHT, 5);
		btnSizer->Add(new wxButton(this, wxID_CANCEL), 0, wxLEFT, 5);
		sizer->Add(btnSizer, 0, wxCENTRE | wxALL, 10);
	}
	sizer->Fit(this);
	SetSizer(sizer);
}

Settings::CodeType SettingOutputDlg::getType() const
{
	return Settings::CodeType(m_typeChoice->GetSelection());
}
