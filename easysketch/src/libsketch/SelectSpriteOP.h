#ifndef _LIBSKETCH_SELECT_SPRITE_OP_H_
#define _LIBSKETCH_SELECT_SPRITE_OP_H_


#include <easy3d.h>

namespace libsketch
{

class StagePanel;

class SelectSpriteOP : public ee::EditOP
{
public:	
	SelectSpriteOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);

	virtual bool OnDraw() const;

	const ee::SpriteSelection& GetSelection() const {
		return *m_selection;
	}
	ee::SpriteSelection& GetSelection() {
		return *m_selection;
	}

private:
	ee::Sprite* SelectByPos(const ivec2& pos) const;

protected:
	StagePanel* m_stage;

private:
	ee::SpriteSelection* m_selection;

}; // SelectSpriteOP

}

#endif // _LIBSKETCH_SELECT_SPRITE_OP_H_