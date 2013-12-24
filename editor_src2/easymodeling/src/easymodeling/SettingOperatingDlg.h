
#ifndef EMODELING_SETTING_OPERATING_DLG_H
#define EMODELING_SETTING_OPERATING_DLG_H

#include <wx/wx.h>

namespace emodeling
{
	class SettingOperatingDlg : public wxDialog
	{
	public:
		SettingOperatingDlg(wxWindow* parent);

	private:
		void onChangeZoomType(wxCommandEvent& event);

	}; // SettingOperatingDlg
}

#endif // EMODELING_SETTING_OPERATING_DLG_H