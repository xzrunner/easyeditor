#ifndef _EASYCOMPLEX_SELECT_SPRITES_OP_H_
#define _EASYCOMPLEX_SELECT_SPRITES_OP_H_

#include <drag2d.h>

namespace ecomplex
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::PropertySettingPanel* propertyPanel, d2d::ViewPanelMgr* view_panel_mgr,
		d2d::AbstractEditCMPT* callback = NULL);

//		virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDClick(int x, int y);

	virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

}; // SelectSpritesOP

}

#endif // _EASYCOMPLEX_SELECT_SPRITES_OP_H_