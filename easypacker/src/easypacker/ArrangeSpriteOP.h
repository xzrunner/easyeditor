#ifndef EPACKER_ARRANGE_SPRITE_OP_H
#define EPACKER_ARRANGE_SPRITE_OP_H

#include <drag2d.h>

namespace epacker
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

#endif // EPACKER_ARRANGE_SPRITE_OP_H