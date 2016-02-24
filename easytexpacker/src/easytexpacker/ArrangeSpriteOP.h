#ifndef _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_
#define _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>

namespace etexpacker
{

class StagePanel;

class ArrangeSpriteOP : public ee::ArrangeSpriteOP<ee::SelectSpritesOP>
{
public:
	ArrangeSpriteOP(StagePanel* editPanel, 
		ee::PropertySettingPanel* property,
		ee::EditCMPT* callback = NULL);
}; // ArrangeSpriteOP

}

#endif // _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_