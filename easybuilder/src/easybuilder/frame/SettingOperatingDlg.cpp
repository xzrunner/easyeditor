//#include "SettingOperatingDlg.h"
//
//
//
//using namespace ebuilder;
//
//SettingOperatingDlg::SettingOperatingDlg(wxWindow* parent)
//	: wxDialog(parent, wxID_ANY, wxT("operating"))
//{
//	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
//	{
//		wxArrayString choices;
//		choices.Add(wxT("mouse"));
//		choices.Add(wxT("mouse + keyboard"));
//
//		wxRadioBox* editChoice = new wxRadioBox(this, wxID_ANY, wxT("scale method"), 
//			wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
//		if (ee::Settings::bZoomOnlyUseMouseWheel)
//			editChoice->SetSelection(0);
//		else
//			editChoice->SetSelection(1);
//		Connect(editChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
//			wxCommandEventHandler(SettingOperatingDlg::onChangeZoomType));
//		sizer->Add(editChoice, 0, wxCENTRE | wxALL, 20);
//	}
//	sizer->Fit(this);
//	SetSizer(sizer);
//}
//
//void SettingOperatingDlg::onChangeZoomType(wxCommandEvent& event)
//{
//	if (event.GetInt() == 0)
//		ee::Settings::bZoomOnlyUseMouseWheel = true;
//	else
//		ee::Settings::bZoomOnlyUseMouseWheel = false;
//}
