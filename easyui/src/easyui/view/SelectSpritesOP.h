#ifndef _EASYUI_SELECT_SPRITES_OP_H_
#define _EASYUI_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace eui
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::PropertySettingPanel* property = NULL, d2d::ViewPanelMgr* view_panel_mgr = NULL,
		d2d::AbstractEditCMPT* callback = NULL);

	virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

}; // SelectSpritesOP

}

#endif // _EASYUI_SELECT_SPRITES_OP_H_