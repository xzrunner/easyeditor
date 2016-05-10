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
	AlphaSettingDlg(wxWindow* parent, s2::Color& color,
		const wxPoint& pos = wxDefaultPosition);

	//
	// interface ColorMonitor
	//
	virtual s2::Color GetColor() const;
	virtual void OnColorChanged();
	virtual void OnColorChanged(const s2::Color& color);

private:
	void InitLayout();

	void SetColor(const s2::Color& color);

private:
	s2::Color& m_color;

	ColorSlider* m_alpha;

}; // AlphaSettingDlg

}

#endif // _EASYEDITOR_ALPHA_SETTING_DLG_H_