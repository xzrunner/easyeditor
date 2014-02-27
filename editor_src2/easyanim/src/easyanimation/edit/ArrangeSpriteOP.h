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
		virtual ~ArrangeSpriteOP();

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);
		
		virtual bool onDraw() const;

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
		const StageSettings& m_settings;

		std::vector<Cross*> m_crosses;
		Cross* m_selected;

	}; // ArrangeSpriteOP
}

