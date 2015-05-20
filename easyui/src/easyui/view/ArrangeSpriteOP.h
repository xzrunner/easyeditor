#ifndef _EASYUI_ARRANGE_SPRITES_OP_H_
#define _EASYUI_ARRANGE_SPRITES_OP_H_

#include <drag2d.h>

#include "SelectSpritesOP.h"

namespace eui
{

class StagePanel;

class ArrangeSpriteOP : public d2d::ArrangeSpriteOP<SelectSpritesOP>
{
public:
	ArrangeSpriteOP(StagePanel* stage, d2d::PropertySettingPanel* property);

	virtual bool onDraw() const;

private:
	

}; // ArrangeSpriteOP

}

#endif // _EASYUI_ARRANGE_SPRITES_OP_H_