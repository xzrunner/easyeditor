#pragma once

#include "SelectSpritesOP.h"
#include "KeyDownHandler.h"

namespace eanim
{
	class StagePanel;

	class ArrangeSpriteOP : public d2d::ArrangeSpriteOP<SelectSpritesOP>
	{
	public:
		ArrangeSpriteOP(StagePanel* stage);
		virtual ~ArrangeSpriteOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);
		
		virtual bool onDraw(const d2d::Screen& scr) const;

		void addCross();
		void delCross();

	private:
		struct Cross
		{
			static const int RADIUS = 10;
			static const int LENGTH = 100;

			Cross();
			void draw(const d2d::Screen& scr) const;
			bool contain(const d2d::Vector& p) const;

			d2d::Vector pos;

		}; // Cross

	private:
		KeyDownHandler m_keyDownHandler;

		std::vector<Cross*> m_crosses;
		Cross* m_selected;

	}; // ArrangeSpriteOP
}

