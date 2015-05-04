#pragma once

#include <wx/wx.h>

namespace eanim
{

	class Controller;
	class CodeSettingDlg : public wxDialog
	{
	public:
		CodeSettingDlg(wxWindow* parent, Controller* ctrl);

	private:
		void onSelectPackerFile(wxCommandEvent& event);

	private:
		Controller* m_ctrl;

		wxTextCtrl* m_packerPath;

	}; // CodeSettingDlg
}

