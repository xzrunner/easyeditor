#include "PropertySettingPanel.h"
#include "StagePanel.h"

#include "frame/Controller.h"

namespace eanim
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: d2d::PropertySettingPanel(parent)
{
}

d2d::IPropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	Controller* ctrl = ((StagePanel*)(m_stage->GetEditPanel()))->GetController();
	return new d2d::SymbolPropertySetting(m_stage, &ctrl->name);
}

}