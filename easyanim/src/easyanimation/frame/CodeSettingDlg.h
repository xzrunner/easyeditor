#pragma once

#include <wx/wx.h>

namespace eanim
{
	class CodeSettingDlg : public wxDialog
	{
	public:
		CodeSettingDlg(wxWindow* parent);

	private:
		void onSelectPackerFile(wxCommandEvent& event);

	private:
		wxTextCtrl* m_packerPath;

	}; // CodeSettingDlg
}

