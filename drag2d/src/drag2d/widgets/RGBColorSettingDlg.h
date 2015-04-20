#ifndef _DRAG2D_RGB_COLOR_SETTING_DLG_H_
#define _DRAG2D_RGB_COLOR_SETTING_DLG_H_

#include "IColorMonitor.h"

#include <wx/wx.h>

namespace d2d
{

class EditPanel;
class ColorSlider;
struct Colorf;

class RGBColorSettingDlg : public wxDialog, public IColorMonitor
{
public:
	RGBColorSettingDlg(EditPanel* editpanel, Colorf& col);

	//
	// interface IColorChanger
	//
	virtual void OnColorChanged();

	Colorf GetColor() const;

private:
	void InitLayout();

	void SetColor(const Colorf& col);

private:
	EditPanel* m_editpanel;

	Colorf& m_color;

	wxPanel* m_color_bg;

	ColorSlider *m_r, *m_g, *m_b;

}; // RGBColorSettingDlg

}

#endif // _DRAG2D_RGB_COLOR_SETTING_DLG_H_