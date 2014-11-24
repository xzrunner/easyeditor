#ifndef _EASYPARTICLE2D_SPECIAL_SETTING_PAGE_H_
#define _EASYPARTICLE2D_SPECIAL_SETTING_PAGE_H_

#include "SettingPageBase.h"

namespace eparticle2d
{

class StageData;

class SpecialSettingPage : public SettingPageBase
{
public:
	SpecialSettingPage(wxWindow* parent, StageData* stage_data);

private:
	void InitLayout();

private:
	StageData* m_stage_data;

}; // SpecialSettingPage

}

#endif // _EASYPARTICLE2D_SPECIAL_SETTING_PAGE_H_