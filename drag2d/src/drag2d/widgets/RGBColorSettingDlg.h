#ifndef _DRAG2D_RGB_COLOR_SETTING_DLG_H_
#define _DRAG2D_RGB_COLOR_SETTING_DLG_H_

#include "IColorMonitor.h"

#include <wx/wx.h>

namespace d2d
{

class ColorSlider;
struct Colorf;

class RGBColorSettingDlg : public wxDialog, public IColorMonitor
{
public:
	RGBColorSettingDlg(wxWindow* parent, const Colorf& col);

	//
	// interface IColorChanger
	//
	virtual void OnColorChanged();

	Colorf GetColor() const;

private:
	void InitLayout();
	void SetColor(const Colorf& col);

private:
	wxPanel* m_color_bg;

	ColorSlider *m_r, *m_g, *m_b;

}; // RGBColorSettingDlg

}

#endif // _DRAG2D_RGB_COLOR_SETTING_DLG_H_