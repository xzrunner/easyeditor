#ifndef _DRAG2D_HSL_COLOR_SETTING_DLG_H_
#define _DRAG2D_HSL_COLOR_SETTING_DLG_H_

#include "common/color_config.h"

#include "IColorMonitor.h"
#include "common/Color.h"

#include <wx/wx.h>

namespace d2d
{

class IColorMonitor;
class ColorSlider;
	
class HSLColorSettingDlg : public wxDialog, public IColorMonitor
{
public:
	HSLColorSettingDlg(wxWindow* parent, IColorMonitor* lsn, 
		const Colorf& col = WHITE, const wxPoint& pos = wxDefaultPosition);

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

	ColorSlider *m_h, *m_s, *m_l;

}; // HSLColorSettingDlg

}

#endif // _DRAG2D_HSL_COLOR_SETTING_DLG_H_