#include "SettingsDialog.h"

#include "common/Settings.h"

namespace d2d
{

SettingsDialog::SettingsDialog(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Settings"))
{
	initLayout();
}

void SettingsDialog::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddSpacer(20);
	sizer->Add(initEditPanel(), 0, wxCENTER);
	sizer->AddSpacer(20);
	sizer->Add(initImagePanel(), 0, wxCENTER);
	SetSizer(sizer);

	sizer->Layout();
	Refresh();
}

wxSizer* SettingsDialog::initEditPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("±à¼­"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("¶ÔÏó²¶×½"));
		check->SetValue(Settings::bSpriteCapture);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeSpriteCapture));
		sizer->Add(check, 0);
	}

	return sizer;
}

wxSizer* SettingsDialog::initImagePanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Í¼Æ¬"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("±ßÔµ²Ã¼ô"));
		check->SetValue(Settings::bSpriteCapture);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeImageEdgeClip));
		sizer->Add(check, 0);
	}

	return sizer;
}

wxSizer* SettingsDialog::initViewPanel()
{
	return NULL;
}

void SettingsDialog::onChangeSpriteCapture(wxCommandEvent& event)
{
	Settings::bSpriteCapture = event.IsChecked();
}

void SettingsDialog::onChangeImageEdgeClip(wxCommandEvent& event)
{
	Settings::bImageEdgeClip = event.IsChecked();
}

}