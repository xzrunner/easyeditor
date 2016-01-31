#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "SymbolPropertySetting.h"

namespace ecomplex
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: ee::PropertySettingPanel(parent)
{
}

ee::PropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	return new SymbolPropertySetting(static_cast<StagePanel*>(m_stage->GetEditPanel())->getSymbol());
}

}