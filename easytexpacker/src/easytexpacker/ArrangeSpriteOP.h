#ifndef _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_
#define _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_

#include <drag2d.h>

namespace etp
{
	class StagePanel;

	class ArrangeSpriteOP : public d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>
	{
	public:
		ArrangeSpriteOP(StagePanel* editPanel, 
			d2d::PropertySettingPanel* propertyPanel,
			d2d::AbstractEditCMPT* callback = NULL);
	}; // ArrangeSpriteOP
}

#endif // _EASYTEXPACKER_ARRANGE_SPRITE_OP_H_