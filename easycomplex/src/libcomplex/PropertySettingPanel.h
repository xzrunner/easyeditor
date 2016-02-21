#ifndef _EASY_EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_
#define _EASY_EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_

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

#endif // _EASY_EASYCOMPLEX_SYMBOL_PROPERTY_SETTING_H_