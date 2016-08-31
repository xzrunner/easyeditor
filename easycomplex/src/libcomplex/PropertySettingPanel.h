#ifndef _EASYCOMPLEX_PROPERTY_SETTING_PANEL_H_
#define _EASYCOMPLEX_PROPERTY_SETTING_PANEL_H_

#include <ee/PropertySettingPanel.h>

namespace ecomplex
{

class PropertySettingPanel : public ee::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

protected:
	virtual ee::PropertySetting* CreateDefaultProperty() const;

}; // PropertySettingPanel

}

#endif // _EASYCOMPLEX_PROPERTY_SETTING_PANEL_H_