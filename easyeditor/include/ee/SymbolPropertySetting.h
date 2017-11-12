#ifndef _EASYEDITOR_SYMBOL_PROPERTY_SETTING_H_
#define _EASYEDITOR_SYMBOL_PROPERTY_SETTING_H_

#include "PropertySetting.h"
#include "Symbol.h"

namespace ee
{

class SymbolPropertySetting : public PropertySetting
{
public:
	SymbolPropertySetting(const std::shared_ptr<Symbol>& sym);
	SymbolPropertySetting(std::string* name, std::string* tag);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

protected:
	std::shared_ptr<Symbol> m_sym;

	std::string* m_name;
	std::string* m_tag;

}; // SymbolPropertySetting

}

#endif // _EASYEDITOR_SYMBOL_PROPERTY_SETTING_H_