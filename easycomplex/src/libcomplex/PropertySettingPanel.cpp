#include "PropertySettingPanel.h"
#include "StagePanel.h"
#include "SymbolPropertySetting.h"

#include <ee/EditPanelImpl.h>

namespace ecomplex
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: ee::PropertySettingPanel(parent)
{
}

ee::PropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	StagePanel* stage = static_cast<StagePanel*>(m_stage->GetEditPanel());
	return new SymbolPropertySetting(stage->GetSymbol());
}

}