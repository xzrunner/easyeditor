#ifndef _EASYPARTICLE2D_EMITTER_SETTING_PAGE_H_
#define _EASYPARTICLE2D_EMITTER_SETTING_PAGE_H_

#include "SettingPageBase.h"

namespace eparticle2d
{

class StageData;
class ISliderCtrl;

class EmitterSettingPage : public SettingPageBase
{
public:
	EmitterSettingPage(wxWindow* parent, StageData* stage_data);

private:
	void InitLayout();

private:
	StageData* m_stage_data;

}; // EmitterSettingPage

}

#endif // _EASYPARTICLE2D_EMITTER_SETTING_PAGE_H_