#ifndef _EASYEDITOR_SETTINGS_DIALOG_H_
#define _EASYEDITOR_SETTINGS_DIALOG_H_

#include <wx/wx.h>

namespace ee
{

class SettingData;

class SettingsDialog : public wxDialog
{
public:
	SettingsDialog(wxWindow* parent);

private:
	void InitLayout();

	wxSizer* initEditPanel();
	wxSizer* initImagePanel();
	wxSizer* initViewPanel();

	void onChangeSpriteCapture(wxCommandEvent& event);
	void onChangeImageEdgeClip(wxCommandEvent& event);
	void onChangeVisibleImageEdge(wxCommandEvent& event);
	void onChangeImageFilterType(wxCommandEvent& event);
	void onChangeFontBackground(wxCommandEvent& event);
	void onChangeFontText(wxCommandEvent& event);
	void onChangeNodeName(wxCommandEvent& event);
	void onChangeShape(wxCommandEvent& event);
	void onChangeTextureEdge(wxCommandEvent& event);
	void onSetBGColor(wxCommandEvent& event);
	void onChangeUpdateSpr(wxCommandEvent& event);
	void onChangeScissor(wxCommandEvent& event);

private:
	SettingData& m_settings;

}; // SettingsDialog

}

#endif // _EASYEDITOR_SETTINGS_DIALOG_H_