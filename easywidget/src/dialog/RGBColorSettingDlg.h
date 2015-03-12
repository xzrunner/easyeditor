#ifndef _EASYWIDGET_RGB_COLOR_SETTING_DLG_H_
#define _EASYWIDGET_RGB_COLOR_SETTING_DLG_H_

#include <easy2d.h>
#include <easygui.h>
#include "IColorMonitor.h"

namespace ewidget
{

class ColorSlider;

class RGBColorSettingDlg : public egui::Dialog, public IColorMonitor
{
public:
	RGBColorSettingDlg(egui::Window* parent, const Colorf& col);

	//
	// interface IColorChanger
	//
	virtual void OnColorChanged();

	Colorf GetColor() const;

private:
	void InitLayout();

	void SetColor(const Colorf& col);

private:
	egui::Panel* m_color_bg;

	ColorSlider *m_r, *m_g, *m_b;

}; // RGBColorSettingDlg

}

#endif // _EASYWIDGET_RGB_COLOR_SETTING_DLG_H_