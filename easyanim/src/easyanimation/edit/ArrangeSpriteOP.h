#ifndef _EASYANIM_ARRANGE_SPRITE_OP_H_
#define _EASYANIM_ARRANGE_SPRITE_OP_H_

#include "SelectSpritesOP.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/CrossGuides.h>

namespace eanim
{

class StagePanel;

class ArrangeSpriteOP : public ee::ArrangeSpriteOP<SelectSpritesOP>
{
public:
	ArrangeSpriteOP(StagePanel* stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	
	virtual bool OnDraw() const override;

	void AddCross();
	void DelCross();

private:
	ee::CrossGuides m_guides;

}; // ArrangeSpriteOP

}

#endif // _EASYANIM_ARRANGE_SPRITE_OP_H_