#ifndef _EASYMODELING_PROPERTY_SETTING_PANEL_H_
#define _EASYMODELING_PROPERTY_SETTING_PANEL_H_

#include <drag2d.h>

namespace emodeling
{

class PropertySettingPanel : public d2d::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

protected:
	virtual void OnSpriteSelected(d2d::Sprite* spr, bool clear);

}; // PropertySettingPanel
 

}

#endif // _EASYMODELING_PROPERTY_SETTING_PANEL_H_