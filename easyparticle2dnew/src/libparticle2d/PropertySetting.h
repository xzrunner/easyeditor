#ifndef _EASYPARTICLE2D_PROPERTY_SETTING_H_
#define _EASYPARTICLE2D_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace eparticle2d
{

class Symbol;

class PropertySetting : public d2d::SymbolPropertySetting
{
public:
	PropertySetting(d2d::EditPanel* editPanel, std::string* name);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

protected:
	virtual void updateProperties(wxPropertyGrid* pg);
	virtual void initProperties(wxPropertyGrid* pg);

}; // PropertySetting

}

#endif // _EASYPARTICLE2D_PROPERTY_SETTING_H_