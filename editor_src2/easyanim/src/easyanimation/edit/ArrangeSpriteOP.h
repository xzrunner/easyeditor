#pragma once

#include "SelectSpritesOP.h"

namespace eanim
{
	class StagePanel;
	class StageSettings;

	class ArrangeSpriteOP : public d2d::ArrangeSpriteOP<SelectSpritesOP>
	{
	public:
		ArrangeSpriteOP(StagePanel* stage);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);
		
		virtual bool onDraw() const;

	private:
		static const int RADIUS = 10;
		static const int LENGTH = 100;

	private:
		const StageSettings& m_settings;

		d2d::Vector m_center;

		bool m_bMoveCenter;

	}; // ArrangeSpriteOP
}

