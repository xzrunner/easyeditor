#ifndef _LR_ARRANGE_SPRITE_IMPL_H_
#define _LR_ARRANGE_SPRITE_IMPL_H_

#include <drag2d.h>

namespace lr
{

class StagePanel;
class RightPopupMenu;

class ArrangeSpriteImpl : public d2d::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, d2d::PropertySettingPanel* property,
		RightPopupMenu* popup);

protected:
	virtual void SetRightPopupMenu(wxMenu& menu, d2d::ISprite* spr);

private:
	RightPopupMenu* m_popup;

}; // ArrangeSpriteImpl

}

#endif // _LR_ARRANGE_SPRITE_IMPL_H_