#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "SymbolPropertySetting.h"

namespace ecomplex
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: d2d::PropertySettingPanel(parent)
{
}

d2d::IPropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	return new SymbolPropertySetting(static_cast<StagePanel*>(m_stage->GetEditPanel())->getSymbol());
}

}