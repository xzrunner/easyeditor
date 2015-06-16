#pragma once

#include "SelectSpritesOP.h"
#include "KeyDownHandler.h"

namespace eanim
{
	class StagePanel;
	class Controller;

	class ArrangeSpriteOP : public d2d::ArrangeSpriteOP<SelectSpritesOP>
	{
	public:
		ArrangeSpriteOP(StagePanel* stage, d2d::PropertySettingPanel* property,
			Controller* ctrl);
		virtual ~ArrangeSpriteOP();

		virtual bool OnKeyDown(int keyCode);
		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseLeftUp(int x, int y);
		virtual bool OnMouseDrag(int x, int y);
		
		virtual bool OnDraw() const;

		void addCross();
		void delCross();

	private:
		struct Cross
		{
			static const int RADIUS = 10;
			static const int LENGTH = 100;

			Cross();
			void draw() const;
			bool contain(const d2d::Vector& p) const;

			d2d::Vector pos;

		}; // Cross

	private:
		KeyDownHandler m_keyDownHandler;

		std::vector<Cross*> m_crosses;
		Cross* m_selected;

	}; // ArrangeSpriteOP
}

