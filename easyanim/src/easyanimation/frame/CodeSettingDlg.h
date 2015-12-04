#ifndef _EASYANIM_CODE_SETTING_DLG_H_
#define _EASYANIM_CODE_SETTING_DLG_H_

#include <wx/wx.h>

namespace eanim
{

class CodeSettingDlg : public wxDialog
{
public:
	CodeSettingDlg(wxWindow* parent);

private:
	void OnSelectTPFile(wxCommandEvent& event);

private:
	wxTextCtrl* m_tp_path;

}; // CodeSettingDlg

}

#endif // _EASYANIM_CODE_SETTING_DLG_H_