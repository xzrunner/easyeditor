#ifndef _EASYEDITOR_SYMBOL_PROPERTY_SETTING_H_
#define _EASYEDITOR_SYMBOL_PROPERTY_SETTING_H_

#include "PropertySetting.h"

namespace ee
{

class Symbol;

class SymbolPropertySetting : public PropertySetting
{
public:
	SymbolPropertySetting(Symbol* sym);
	SymbolPropertySetting(std::string* name, std::string* tag);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

protected:
	Symbol* m_sym;

	std::string* m_name;
	std::string* m_tag;

}; // SymbolPropertySetting

}

#endif // _EASYEDITOR_SYMBOL_PROPERTY_SETTING_H_