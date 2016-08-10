#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "MotionTrail.h"

#include <ee/SymbolPropertySetting.h>

namespace etrail
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: ee::PropertySettingPanel(parent)
	, m_stage(NULL)
{
}

ee::PropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	if (m_stage && m_stage->m_trail) {
		return new ee::SymbolPropertySetting(&m_stage->m_trail->name, NULL);
	} else {
		return NULL;
	}
}

}