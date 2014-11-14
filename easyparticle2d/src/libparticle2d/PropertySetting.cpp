#include "PropertySetting.h"
#include "Symbol.h"

namespace eparticle2d
{

PropertySetting::PropertySetting(d2d::EditPanel* editPanel, std::string* name)
	: SymbolPropertySetting(editPanel, name)
{
}

void PropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	SymbolPropertySetting::onPropertyGridChange(name, value);

	if (value.IsNull())
		return;
}

void PropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	SymbolPropertySetting::enablePropertyGrid(panel, bEnable);
}

void PropertySetting::updateProperties(wxPropertyGrid* pg)
{
	SymbolPropertySetting::updateProperties(pg);
}

void PropertySetting::initProperties(wxPropertyGrid* pg)
{
	SymbolPropertySetting::initProperties(pg);
}

}