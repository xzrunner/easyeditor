#pragma once

#include <drag2d.h>

namespace eanim
{
	class StagePanel;

	class ArrangeSpriteOP : public d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>
	{
	public:
		ArrangeSpriteOP(StagePanel* editPanel);

		virtual bool onMouseLeftDClick(int x, int y);

	private:
		StagePanel* m_editPanel;

	}; // ArrangeSpriteOP
}

