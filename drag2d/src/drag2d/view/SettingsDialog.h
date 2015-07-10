#ifndef _DRAG2D_SETTINGS_DIALOG_H_
#define _DRAG2D_SETTINGS_DIALOG_H_

#include <wx/wx.h>

namespace d2d
{

struct SettingData;

class SettingsDialog : public wxDialog
{
public:
	SettingsDialog(wxWindow* parent);

private:
	void initLayout();

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

private:
	SettingData& m_settings;

}; // SettingsDialog

}

#endif // _DRAG2D_SETTINGS_DIALOG_H_