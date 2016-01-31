#ifndef _EASYEDITOR_ALPHA_SETTING_DLG_H_
#define _EASYEDITOR_ALPHA_SETTING_DLG_H_

#include "ColorMonitor.h"

#include <wx/wx.h>

namespace ee
{

class ColorSlider;

class AlphaSettingDlg : public wxDialog, public ColorMonitor
{
public:
	AlphaSettingDlg(wxWindow* parent, Colorf& color,
		const wxPoint& pos = wxDefaultPosition);

	//
	// interface ColorMonitor
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

#endif // _EASYEDITOR_ALPHA_SETTING_DLG_H_