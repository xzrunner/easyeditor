#ifndef _EASYANIM_PROPERTY_SETTING_PANEL_H_
#define _EASYANIM_PROPERTY_SETTING_PANEL_H_

#include <drag2d.h>

namespace eanim
{

class PropertySettingPanel : public d2d::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

protected:
	virtual d2d::IPropertySetting* CreateDefaultProperty() const;

}; // PropertySettingPanel

}

#endif // _EASYANIM_PROPERTY_SETTING_PANEL_H_