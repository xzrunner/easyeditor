#ifndef _LIBSKETCH_SELECT_SPRITE_OP_H_
#define _LIBSKETCH_SELECT_SPRITE_OP_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class StagePanel;

class SelectSpriteOP : public d2d::AbstractEditOP
{
public:	
	SelectSpriteOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);

	virtual bool OnDraw() const;

	const d2d::SpriteSelection& GetSelection() const {
		return *m_selection;
	}
	d2d::SpriteSelection& GetSelection() {
		return *m_selection;
	}

private:
	d2d::Sprite* SelectByPos(const ivec2& pos) const;

protected:
	StagePanel* m_stage;

private:
	d2d::SpriteSelection* m_selection;

}; // SelectSpriteOP

}

#endif // _LIBSKETCH_SELECT_SPRITE_OP_H_