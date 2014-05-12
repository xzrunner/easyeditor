
#include "SettingGameDlg.h"

#include "dataset/Game.h"

using namespace ebuilder;

SettingGameDlg::SettingGameDlg(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("game"))
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	{
		wxSizer* sizer = initNamePanel();
		topSizer->Add(sizer, 0, wxCENTRE | wxALL, 20);
	}
	{
		wxSizer* sizer = initSizePanel();
		topSizer->Add(sizer, 0, wxCENTRE | wxALL, 20);
	}
	{
		wxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
		btnSizer->Add(new wxButton(this, wxID_OK), 0, wxRIGHT, 5);
		btnSizer->Add(new wxButton(this, wxID_CANCEL), 0, wxLEFT, 5);
		topSizer->Add(btnSizer, 0, wxCENTRE | wxALL, 10);
	}
	topSizer->Fit(this);
	SetSizer(topSizer);
}

wxString SettingGameDlg::getName() const
{
	return m_nameText->GetLineText(0);
}

int SettingGameDlg::getWidth() const
{
	return wxVariant(m_widthText->GetLineText(0)).GetInteger();
}

int SettingGameDlg::getHeight() const
{
	return wxVariant(m_heightText->GetLineText(0)).GetInteger();
}

wxSizer* SettingGameDlg::initNamePanel()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Name:")));
	sizer->AddSpacer(10);

	m_nameText = new wxTextCtrl(this, wxID_ANY, Game::NAME,
		wxDefaultPosition, wxSize(150, -1));
	sizer->Add(m_nameText);

	return sizer;
}

wxSizer* SettingGameDlg::initSizePanel()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Size:")));
	sizer->AddSpacer(10);

	m_widthText = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(Game::WIDTH),
		wxDefaultPosition, wxSize(50, -1));
	sizer->Add(m_widthText);

	sizer->Add(new wxStaticText(this, wxID_ANY, wxT(" * ")));

	m_heightText = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(Game::HEIGHT),
		wxDefaultPosition, wxSize(50, -1));
	sizer->Add(m_heightText);

	return sizer;
}
