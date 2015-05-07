#ifndef _LR_SETTINGS_DIALOG_H_
#define _LR_SETTINGS_DIALOG_H_

#include <wx/wx.h>

namespace lr
{

class StagePanel;

class SettingDialog : public wxDialog
{
public:
	SettingDialog(wxWindow* parent, StagePanel* stage);

private:
	void InitLayout();

	void OnViewSizeChanged(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	wxTextCtrl *m_width_ctrl, *m_height_ctrl;

}; // SettingDialog

}

#endif // _LR_SETTINGS_DIALOG_H_