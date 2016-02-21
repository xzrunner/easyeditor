#ifndef _EASYANIM_PROPERTY_SETTING_PANEL_H_
#define _EASYANIM_PROPERTY_SETTING_PANEL_H_

#include <ee/PropertySettingPanel.h>

namespace eanim
{

class PropertySettingPanel : public ee::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

protected:
	virtual ee::PropertySetting* CreateDefaultProperty() const;

}; // PropertySettingPanel

}

#endif // _EASYANIM_PROPERTY_SETTING_PANEL_H_