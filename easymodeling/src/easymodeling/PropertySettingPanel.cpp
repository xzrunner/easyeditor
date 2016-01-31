#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "WorldPropertySetting.h"
#include "BodyPropertySetting.h"

namespace emodeling
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: ee::PropertySettingPanel(parent)
{
}

void PropertySettingPanel::OnSpriteSelected(ee::Sprite* spr, bool clear)
{
	assert(m_stage);
	if (spr) {
		SetPropertySetting(new BodyPropertySetting(m_stage, spr));
	} else {
		SetPropertySetting(new WorldPropertySetting(m_stage));
	}
}

}