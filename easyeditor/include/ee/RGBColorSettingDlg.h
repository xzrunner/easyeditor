#ifndef _EASYEDITOR_RGB_COLOR_SETTING_DLG_H_
#define _EASYEDITOR_RGB_COLOR_SETTING_DLG_H_

#include "color_config.h"
#include "ColorMonitor.h"

#include <wx/wx.h>

namespace ee
{

class ColorMonitor;
class ColorSlider;

class RGBColorSettingDlg : public wxDialog, public ColorMonitor
{
public:
	RGBColorSettingDlg(wxWindow* parent, ColorMonitor* lsn, const s2::Color& col,
		const wxPoint& pos = wxDefaultPosition);

	//
	// interface ColorMonitor
	//
	virtual s2::Color GetColor() const override;
	virtual void OnColorChanged() override;
	virtual void OnColorChanged(const s2::Color& col) override;

private:
	void InitLayout();

	void SetColor(const s2::Color& col);

private:
	ColorMonitor* m_lsn;

	wxPanel* m_color_bg;

	ColorSlider *m_r, *m_g, *m_b;

}; // RGBColorSettingDlg

}

#endif // _EASYEDITOR_RGB_COLOR_SETTING_DLG_H_