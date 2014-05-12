#ifndef _EASYCOMPLEX_SETTINGS_DIALOG_H_
#define _EASYCOMPLEX_SETTINGS_DIALOG_H_

#include <drag2d.h>

namespace ecomplex
{

class SettingsDialog : public d2d::SettingsDialog
{
public:
	SettingsDialog(wxWindow* parent);

private:
	void initLayout();

	wxSizer* initViewPanel();

	void onChangeVisibleCross(wxCommandEvent& event);
	void onChangeVisibleRect(wxCommandEvent& event);

}; // SettingsDialog

}

#endif // _EASYCOMPLEX_SETTINGS_DIALOG_H_
