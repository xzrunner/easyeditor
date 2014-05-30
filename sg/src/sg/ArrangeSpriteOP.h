#ifndef _SG_ARRANGE_SPRITE_OP_H_
#define _SG_ARRANGE_SPRITE_OP_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;

class ArrangeSpriteOP : public d2d::ArrangeSpriteFixOP
{
public:
	ArrangeSpriteOP(StagePanel* stage, d2d::PropertySettingPanel* property);

}; // ArrangeSpriteOP

}

#endif // _SG_ARRANGE_SPRITE_OP_H_