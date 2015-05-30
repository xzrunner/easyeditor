#include "SymbolPropertySetting.h"
#include "Symbol.h"

namespace eparticle2d
{

SymbolPropertySetting::SymbolPropertySetting(d2d::EditPanel* editPanel, std::string* name)
	: d2d::SymbolPropertySetting(editPanel, name)
{
}

void SymbolPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SymbolPropertySetting::OnPropertyGridChange(name, value);

	if (value.IsNull())
		return;
}

void SymbolPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	d2d::SymbolPropertySetting::UpdateProperties(pg);
}

void SymbolPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	d2d::SymbolPropertySetting::InitProperties(pg);
}

}