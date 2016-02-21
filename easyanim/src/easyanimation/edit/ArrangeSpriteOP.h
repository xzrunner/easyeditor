#ifndef _EASYANIM_ARRANGE_SPRITE_OP_H_
#define _EASYANIM_ARRANGE_SPRITE_OP_H_

#include "SelectSpritesOP.h"

#include <ee/ArrangeSpriteOP.h>

namespace eanim
{

class StagePanel;

class ArrangeSpriteOP : public ee::ArrangeSpriteOP<SelectSpritesOP>
{
public:
	ArrangeSpriteOP(StagePanel* stage);
	virtual ~ArrangeSpriteOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);
	
	virtual bool OnDraw() const;

	void AddCross();
	void DelCross();

private:
	struct Cross
	{
		static const int RADIUS = 10;
		static const int LENGTH = 100;

		Cross();
		void Draw() const;
		bool Contain(const ee::Vector& p) const;

		ee::Vector pos;

	}; // Cross

private:
	std::vector<Cross*> m_crosses;
	Cross* m_selected;

}; // ArrangeSpriteOP

}

#endif // _EASYANIM_ARRANGE_SPRITE_OP_H_