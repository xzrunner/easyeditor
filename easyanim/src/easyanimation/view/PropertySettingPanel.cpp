#include "PropertySettingPanel.h"

#include "dataset/DataMgr.h"

namespace eanim
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: ee::PropertySettingPanel(parent)
{
}

ee::PropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	DataMgr* mgr = DataMgr::Instance();
	return new ee::SymbolPropertySetting(&mgr->name, &mgr->tag);
}

}