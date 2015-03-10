#ifndef _DRAG2D_HSL_COLOR_SETTING_DLG_H_
#define _DRAG2D_HSL_COLOR_SETTING_DLG_H_

#include "IColorMonitor.h"

#include <wx/wx.h>

namespace d2d
{

class ColorSlider;
	
class HSLColorSettingDlg : public wxDialog, public IColorMonitor
{
public:
	HSLColorSettingDlg(wxWindow* parent);

	//
	// interface IColorChanger
	//
	virtual void OnColorChanged();

private:
	void InitLayout();

private:
	wxPanel* m_color_bg;

	ColorSlider *m_hue, *m_saturation, *m_lightness;

}; // HSLColorSettingDlg

}

#endif // _DRAG2D_HSL_COLOR_SETTING_DLG_H_