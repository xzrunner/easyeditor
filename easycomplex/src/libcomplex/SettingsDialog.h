#ifndef _EASYCOMPLEX_SETTINGS_DIALOG_H_
#define _EASYCOMPLEX_SETTINGS_DIALOG_H_



namespace ecomplex
{

class SettingsDialog : public ee::SettingsDialog
{
public:
	SettingsDialog(wxWindow* parent);

private:
	void InitLayout();

	wxSizer* initViewPanel();

	void onChangeVisibleCross(wxCommandEvent& event);
	void onChangeVisibleRect(wxCommandEvent& event);

}; // SettingsDialog

}

#endif // _EASYCOMPLEX_SETTINGS_DIALOG_H_
