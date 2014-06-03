#pragma once

#include "ArrangeSpriteOP.h"
#include "SelectSpritesOP.h"

namespace d2d
{
	class ArrangeSpriteFixOP : public ArrangeSpriteOP<SelectSpritesOP>
	{
	public:
		ArrangeSpriteFixOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
			PropertySettingPanel* propertyPanel = NULL, AbstractEditCMPT* callback = NULL,
			bool isDeformOpen = true, bool isAutoAlignOpen = true);

		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseRightUp(int x, int y);

	private:
		// todo cost!
		void fixSpritesLocation(const std::vector<ISprite*>& sprites);

	private:
		MultiSpritesImpl* m_spritesImpl;

	}; // ArrangeSpriteFixOP
}

