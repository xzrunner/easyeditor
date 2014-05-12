
#ifndef EBUILDER_SETTING_GAME_DLG_H
#define EBUILDER_SETTING_GAME_DLG_H

#include <wx/wx.h>

namespace ebuilder
{	
	class SettingGameDlg : public wxDialog
	{
	public:
		SettingGameDlg(wxWindow* parent);

		wxString getName() const;

		int getWidth() const;
		int getHeight() const;

	private:
		wxSizer* initNamePanel();
		wxSizer* initSizePanel();

	private:
		wxTextCtrl* m_nameText;

		wxTextCtrl* m_widthText;
		wxTextCtrl* m_heightText;

	}; // SettingGameDlg
}

#endif // EBUILDER_SETTING_GAME_DLG_H
