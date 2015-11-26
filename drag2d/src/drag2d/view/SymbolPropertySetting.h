#ifndef _DRAG2D_SYMBOL_PROPERTY_SETTING_H_
#define _DRAG2D_SYMBOL_PROPERTY_SETTING_H_

#include "IPropertySetting.h"

namespace d2d
{

class ISymbol;

class SymbolPropertySetting : public IPropertySetting
{
public:
	SymbolPropertySetting(ISymbol* symbol);
	SymbolPropertySetting(std::string* name);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

protected:
	ISymbol* m_symbol;

	std::string* m_name;

}; // SymbolPropertySetting

}

#endif // _DRAG2D_SYMBOL_PROPERTY_SETTING_H_