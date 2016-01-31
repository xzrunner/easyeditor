#include "SettingsDialog.h"
#include "Settings.h"

namespace ecomplex
{

SettingsDialog::SettingsDialog(wxWindow* parent)
	: ee::SettingsDialog(parent)
{
	InitLayout();
}

void SettingsDialog::InitLayout()
{
	wxSizer* sizer = GetSizer();
	sizer->Add(initViewPanel(), 0, wxCENTER);
}

wxSizer* SettingsDialog::initViewPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("显示"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("中心点"));
		check->SetValue(Settings::bVisibleBGCross);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeVisibleCross));
		sizer->Add(check, 0);
	}
	sizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("屏幕框"));
		check->SetValue(Settings::bVisibleBGRect);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeVisibleRect));
		sizer->Add(check, 0);
	}
	return sizer;
}

void SettingsDialog::onChangeVisibleCross(wxCommandEvent& event)
{
	Settings::bVisibleBGCross = event.IsChecked();
}

void SettingsDialog::onChangeVisibleRect(wxCommandEvent& event)
{
	Settings::bVisibleBGRect = event.IsChecked();
}

}