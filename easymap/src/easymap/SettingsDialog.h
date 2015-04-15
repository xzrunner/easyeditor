#ifndef _EASYMAP_SETTINGS_DIALOG_H_
#define _EASYMAP_SETTINGS_DIALOG_H_

#include <wx/wx.h>

namespace emap
{

class SettingDialog : public wxDialog
{
public:
	SettingDialog(wxWindow* parent);

private:
	void InitLayout();

	void OnViewSizeChanged(wxCommandEvent& event);

private:
	wxTextCtrl *m_width_ctrl, *m_height_ctrl;

}; // SettingDialog

}

#endif // _EASYMAP_SETTINGS_DIALOG_H_