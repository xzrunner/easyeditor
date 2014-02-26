#ifndef _DRAG2D_SETTINGS_DIALOG_H_
#define _DRAG2D_SETTINGS_DIALOG_H_

#include <wx/wx.h>

namespace d2d
{

class SettingsDialog : public wxDialog
{
public:
	SettingsDialog(wxWindow* parent);

private:
	void initLayout();

	void onChangeSpriteCapture(wxCommandEvent& event);

}; // SettingsDialog

}

#endif // _DRAG2D_SETTINGS_DIALOG_H_