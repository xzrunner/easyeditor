#ifndef _EASYEDITOR_HSL_COLOR_SETTING_DLG_H_
#define _EASYEDITOR_HSL_COLOR_SETTING_DLG_H_

#include "color_config.h"
#include "ColorMonitor.h"

#include <wx/wx.h>

namespace ee
{

class ColorMonitor;
class ColorSlider;
	
class HSLColorSettingDlg : public wxDialog, public ColorMonitor
{
public:
	HSLColorSettingDlg(wxWindow* parent, ColorMonitor* lsn, 
		const pt2::Color& col = WHITE, const wxPoint& pos = wxDefaultPosition);

	//
	// interface ColorMonitor
	//
	virtual pt2::Color GetColor() const override;
	virtual void OnColorChanged() override;
	virtual void OnColorChanged(const pt2::Color& col) override;

private:
	void InitLayout();

	void SetColor(const pt2::Color& col);

private:
	ColorMonitor* m_lsn;

	wxPanel* m_color_bg;

	ColorSlider *m_h_ctrl, *m_s_ctrl, *m_l_ctrl;
	float m_h, m_s, m_l;

}; // HSLColorSettingDlg

}

#endif // _EASYEDITOR_HSL_COLOR_SETTING_DLG_H_