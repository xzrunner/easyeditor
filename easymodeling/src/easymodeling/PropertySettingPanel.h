#ifndef _EASYMODELING_PROPERTY_SETTING_PANEL_H_
#define _EASYMODELING_PROPERTY_SETTING_PANEL_H_

#include <drag2d.h>

namespace emodeling
{

class PropertySettingPanel : public d2d::PropertySettingPanel
{
public:
	PropertySettingPanel(wxWindow* parent);

	//
	//	interface ISpriteViewPanel
	//
	virtual void SelectSprite(d2d::ISprite* spr);

}; // PropertySettingPanel
 

}

#endif // _EASYMODELING_PROPERTY_SETTING_PANEL_H_