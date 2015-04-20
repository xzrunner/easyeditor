#ifndef _DRAG2D_HSL_COLOR_SETTING_DLG_H_
#define _DRAG2D_HSL_COLOR_SETTING_DLG_H_

#include "IColorMonitor.h"
#include "common/Color.h"

#include <wx/wx.h>

namespace d2d
{

class EditPanel;
class ColorSlider;
	
class HSLColorSettingDlg : public wxDialog, public IColorMonitor
{
public:
	HSLColorSettingDlg(EditPanel* editpanel, Colorf& col, const wxPoint& pos = wxDefaultPosition);

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

	ColorSlider *m_h, *m_s, *m_l;

}; // HSLColorSettingDlg

}

#endif // _DRAG2D_HSL_COLOR_SETTING_DLG_H_