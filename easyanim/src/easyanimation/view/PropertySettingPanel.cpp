#include "PropertySettingPanel.h"

#include "dataset/DataMgr.h"

namespace eanim
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: d2d::PropertySettingPanel(parent)
{
}

d2d::IPropertySetting* PropertySettingPanel::CreateDefaultProperty() const
{
	DataMgr* mgr = DataMgr::Instance();
	return new d2d::SymbolPropertySetting(&mgr->name, &mgr->tag);
}

}