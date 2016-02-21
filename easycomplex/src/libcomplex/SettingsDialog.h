#ifndef _EASY_EASYCOMPLEX_SETTINGS_DIALOG_H_
#define _EASY_EASYCOMPLEX_SETTINGS_DIALOG_H_

#include <ee/SettingsDialog.h>

namespace ecomplex
{

class SettingsDialog : public ee::SettingsDialog
{
public:
	SettingsDialog(wxWindow* parent);

private:
	void InitLayout();

	wxSizer* initViewPanel();

	void onChangeVisibleCross(wxCommandEvent& event);
	void onChangeVisibleRect(wxCommandEvent& event);

}; // SettingsDialog

}

#endif // _EASY_EASYCOMPLEX_SETTINGS_DIALOG_H_
