#ifndef _EASYPARTICLE2D_SYMBOL_PROPERTY_SETTING_H_
#define _EASYPARTICLE2D_SYMBOL_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace eparticle2d
{

class Symbol;

class SymbolPropertySetting : public d2d::SymbolPropertySetting
{
public:
	SymbolPropertySetting(d2d::EditPanel* editPanel, std::string* name);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // SymbolPropertySetting

}

#endif // _EASYPARTICLE2D_SYMBOL_PROPERTY_SETTING_H_