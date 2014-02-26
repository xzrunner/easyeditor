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
//	sizer->AddSpacer(20);

	wxCheckBox* checkCross = new wxCheckBox(this, wxID_ANY, wxT("¶ÔÏó²¶×½"));
	checkCross->SetValue(Settings::bSpriteCapture);
	Connect(checkCross->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
		wxCommandEventHandler(SettingsDialog::onChangeSpriteCapture));
	sizer->Add(checkCross, 0);

// 	sizer->Layout();
// 	Refresh();
}

void SettingsDialog::onChangeSpriteCapture(wxCommandEvent& event)
{
	Settings::bSpriteCapture = event.IsChecked();
}

}