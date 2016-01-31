//
//#include "SettingOperatingDlg.h"
//
//
//
//using namespace emodeling;
//
//SettingOperatingDlg::SettingOperatingDlg(wxWindow* parent)
//	: wxDialog(parent, wxID_ANY, wxT("Operating Setting"))
//{
//	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
//	{
//		wxArrayString choices;
//		choices.Add(wxT("Mouse"));
//		choices.Add(wxT("Mouse + Keyboard"));
//
//		wxRadioBox* editChoice = new wxRadioBox(this, wxID_ANY, wxT("Scale type"), wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
//		if (ee::SettingData::bZoomOnlyUseMouseWheel)
//			editChoice->SetSelection(0);
//		else
//			editChoice->SetSelection(1);
//		Connect(editChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(SettingOperatingDlg::onChangeZoomType));
//		sizer->Add(editChoice, 0, wxCENTRE | wxALL, 20);
//	}
//	SetSizer(sizer);
//}
//
//void SettingOperatingDlg::onChangeZoomType(wxCommandEvent& event)
//{
//	if (event.GetInt() == 0)
//		ee::SettingData::bZoomOnlyUseMouseWheel = true;
//	else
//		ee::SettingData::bZoomOnlyUseMouseWheel = false;
//}