
#ifndef EBUILDER_SETTING_OUTPUT_DLG_H
#define EBUILDER_SETTING_OUTPUT_DLG_H

#include "Settings.h"

#include <wx/wx.h>

namespace ebuilder
{
	class SettingOutputDlg : public wxDialog
	{
	public:
		SettingOutputDlg(wxWindow* parent);

		Settings::CodeType getType() const;

	private:
		wxRadioBox* m_typeChoice;

	}; // SettingOutputDlg
}

#endif // EBUILDER_SETTING_OUTPUT_DLG_H
