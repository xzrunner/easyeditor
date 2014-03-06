#pragma once

#include <drag2d.h>

namespace escale9
{
	class ComposeOP : public d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>
	{
	public:
		ComposeOP();

		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseDrag(int x, int y) { return false; }

		virtual bool onActive();

	private:

	}; // ComposeOP
}

