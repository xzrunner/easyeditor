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
		const s2::Color& col = WHITE, const wxPoint& pos = wxDefaultPosition);

	//
	// interface ColorMonitor
	//
	virtual s2::Color GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const s2::Color& col);

private:
	void InitLayout();

	void SetColor(const s2::Color& col);

private:
	ColorMonitor* m_lsn;

	wxPanel* m_color_bg;

	ColorSlider *m_h_ctrl, *m_s_ctrl, *m_l_ctrl;
	float m_h, m_s, m_l;

}; // HSLColorSettingDlg

}

#endif // _EASYEDITOR_HSL_COLOR_SETTING_DLG_H_