#ifndef _EASYANIM_ARRANGE_SPRITE_OP_H_
#define _EASYANIM_ARRANGE_SPRITE_OP_H_

#include "SelectSpritesOP.h"
#include "KeyDownHandler.h"

namespace eanim
{

class StagePanel;

class ArrangeSpriteOP : public d2d::ArrangeSpriteOP<SelectSpritesOP>
{
public:
	ArrangeSpriteOP(StagePanel* stage, d2d::PropertySettingPanel* property,
		LayersMgr* layers);
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
		bool Contain(const d2d::Vector& p) const;

		d2d::Vector pos;

	}; // Cross

private:
	KeyDownHandler m_kd_handler;

	std::vector<Cross*> m_crosses;
	Cross* m_selected;

}; // ArrangeSpriteOP

}

#endif // _EASYANIM_ARRANGE_SPRITE_OP_H_