#ifndef _DRAG2D_ALPHA_SETTING_DLG_H_
#define _DRAG2D_ALPHA_SETTING_DLG_H_

#include "common/color_config.h"

#include "IColorMonitor.h"

#include <wx/wx.h>

namespace d2d
{

class ColorSlider;

class AlphaSettingDlg : public wxDialog, public IColorMonitor
{
public:
	AlphaSettingDlg(wxWindow* parent, Colorf& color,
		const wxPoint& pos = wxDefaultPosition);

	//
	// interface IColorMonitor
	//
	virtual Colorf GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const Colorf& color);

private:
	void InitLayout();

	void SetColor(const Colorf& color);

private:
	Colorf& m_color;

	ColorSlider* m_alpha;

}; // AlphaSettingDlg

}

#endif // _DRAG2D_ALPHA_SETTING_DLG_H_