#ifndef _DRAG2D_ARRANGE_SPRITE_FIX_OP_H_
#define _DRAG2D_ARRANGE_SPRITE_FIX_OP_H_

#include "ArrangeSpriteOP.h"
#include "SelectSpritesOP.h"

namespace d2d
{

class ArrangeSpriteFixOP : public ArrangeSpriteOP<SelectSpritesOP>
{
public:
	ArrangeSpriteFixOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* spritesImpl,
		PropertySettingPanel* propertyPanel = NULL, AbstractEditCMPT* callback = NULL, 
		const ArrangeSpriteConfig& cfg = ArrangeSpriteConfig());

	virtual bool OnMouseLeftUp(int x, int y);
	//virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);

private:
	// todo cost!
	void fixSpritesLocation(const std::vector<ISprite*>& sprites);

	void FixSpritesLocation();

}; // ArrangeSpriteFixOP

}

#endif // _DRAG2D_ARRANGE_SPRITE_FIX_OP_H_