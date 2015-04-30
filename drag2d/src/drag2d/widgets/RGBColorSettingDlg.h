#ifndef _DRAG2D_RGB_COLOR_SETTING_DLG_H_
#define _DRAG2D_RGB_COLOR_SETTING_DLG_H_

#include "common/color_config.h"

#include "IColorMonitor.h"

#include <wx/wx.h>

namespace d2d
{

class IColorMonitor;
class ColorSlider;

class RGBColorSettingDlg : public wxDialog, public IColorMonitor
{
public:
	RGBColorSettingDlg(wxWindow* parent, IColorMonitor* lsn, const Colorf& col);

	//
	// interface IColorMonitor
	//
	virtual Colorf GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const Colorf& col);

private:
	void InitLayout();

	void SetColor(const Colorf& col);

private:
	IColorMonitor* m_lsn;

	wxPanel* m_color_bg;

	ColorSlider *m_r, *m_g, *m_b;

}; // RGBColorSettingDlg

}

#endif // _DRAG2D_RGB_COLOR_SETTING_DLG_H_