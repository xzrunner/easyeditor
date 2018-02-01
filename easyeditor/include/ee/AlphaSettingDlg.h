#ifndef _EASYEDITOR_ALPHA_SETTING_DLG_H_
#define _EASYEDITOR_ALPHA_SETTING_DLG_H_

#include "ColorMonitor.h"
#include "Sprite.h"

#include <wx/wx.h>

namespace ee
{

class ColorSlider;

class AlphaSettingDlg : public wxDialog, public ColorMonitor
{
public:
	AlphaSettingDlg(wxWindow* parent, const SprPtr& spr,
		const wxPoint& pos = wxDefaultPosition);

	//
	// interface ColorMonitor
	//
	virtual pt2::Color GetColor() const override;
	virtual void OnColorChanged() override;
	virtual void OnColorChanged(const pt2::Color& color) override;

private:
	void InitLayout();

	void SetColor(const pt2::Color& color);

private:
	SprPtr m_spr;

	ColorSlider* m_alpha;

}; // AlphaSettingDlg

}

#endif // _EASYEDITOR_ALPHA_SETTING_DLG_H_