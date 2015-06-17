#ifndef _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_
#define _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace ecomplex
{

class PropertySettingPanel : public d2d::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

protected:
	virtual d2d::IPropertySetting* CreateDefaultProperty() const;

}; // PropertySettingPanel

}

#endif // _EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_