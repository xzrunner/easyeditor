#ifndef _EASYUI_SETTINGS_DIALOG_H_
#define _EASYUI_SETTINGS_DIALOG_H_

#include <wx/wx.h>

namespace eui
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

#endif // _EASYUI_SETTINGS_DIALOG_H_