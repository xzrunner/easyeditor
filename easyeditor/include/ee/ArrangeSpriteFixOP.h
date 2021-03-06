#ifndef _EASYEDITOR_ARRANGE_SPRITE_FIX_OP_H_
#define _EASYEDITOR_ARRANGE_SPRITE_FIX_OP_H_

#include "ArrangeSpriteOP.h"
#include "SelectSpritesOP.h"

namespace ee
{

class ArrangeSpriteFixOP : public ArrangeSpriteOP<SelectSpritesOP>
{
public:
	ArrangeSpriteFixOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl,
		PropertySettingPanel* property = NULL, EditCMPT* callback = NULL, 
		const ArrangeSpriteConfig& cfg = ArrangeSpriteConfig());

	virtual bool OnMouseLeftUp(int x, int y) override;
	//virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;

private:
	// todo cost!
	void FixSpritesLocation(const std::vector<SprPtr>& sprs);

	void FixSpritesLocation();

}; // ArrangeSpriteFixOP

}

#endif // _EASYEDITOR_ARRANGE_SPRITE_FIX_OP_H_