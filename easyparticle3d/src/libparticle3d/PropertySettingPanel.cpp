#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "SymbolPropertySetting.h"

namespace eparticle3d
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: ee::PropertySettingPanel(parent)
	, m_stage(NULL)
{
}

ee::PropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	if (m_stage && m_stage->m_ps) {
		return new SymbolPropertySetting(m_stage->m_ps);
	} else {
		return NULL;
	}
}

}