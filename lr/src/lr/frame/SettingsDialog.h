#ifndef _LR_SETTINGS_DIALOG_H_
#define _LR_SETTINGS_DIALOG_H_

#include <ee/Sprite.h>

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

	void OnMapSizeChanged(wxCommandEvent& event);
	void OnViewSizeChanged(wxCommandEvent& event);
	void OnChangeTerrain2DAnim(wxCommandEvent& event);
	void OnChangeAllLayersVisibleEditable(wxCommandEvent& event);
	void OnChangeSpecialLayerFlag(wxCommandEvent& event);

	void OnChangeScreenMultiColor(wxCommandEvent& event);
	void OnChangeScreenAddColor(wxCommandEvent& event);

	void OnChangeGradingTexture(wxCommandEvent& event);

	void SetTerrain2dUpdate(const ee::SprPtr& spr, bool open);

private:
	StagePanel* m_stage;

	wxTextCtrl *m_map_width_ctrl, *m_map_height_ctrl;
	wxTextCtrl *m_view_width_ctrl, *m_view_height_ctrl,
		*m_view_offset_x, *m_view_offset_y;

}; // SettingDialog

}

#endif // _LR_SETTINGS_DIALOG_H_