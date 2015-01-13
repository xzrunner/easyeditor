#ifndef _EASYPARTICLE2D_SETTING_PAGE_BASE_H_
#define _EASYPARTICLE2D_SETTING_PAGE_BASE_H_

#include <wx/wx.h>
#include <json/json.h>

namespace eparticle2d
{

class ISliderCtrl;

class SettingPageBase : public wxPanel
{
public:
	SettingPageBase(wxWindow* parent);

	virtual void Load(const Json::Value& val, int version);
	virtual void Store(Json::Value& val) const;

	virtual void ResetUIFromPS();
	virtual void InitPSFromUI();

protected:
	std::vector<ISliderCtrl*> m_sliders;

}; // SettingPageBase

}

#endif // _EASYPARTICLE2D_SETTING_PAGE_BASE_H_