#ifndef _EASYSCALE9_SYMBOL_PROPERTY_SETTING_H_
#define _EASYSCALE9_SYMBOL_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace escale9
{

class Symbol;

class SymbolPropertySetting : public d2d::IPropertySetting
{
public:
	SymbolPropertySetting(d2d::EditPanel* editPanel, Symbol* symbol);

	virtual void updatePanel(d2d::PropertySettingPanel* panel);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
	virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

private:
	Symbol* m_symbol;

}; // SymbolPropertySetting

}

#endif // _EASYSCALE9_SYMBOL_PROPERTY_SETTING_H_