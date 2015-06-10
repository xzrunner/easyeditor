#ifndef _DRAG2D_MOVE_SPRITE_STATE_H_
#define _DRAG2D_MOVE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class MoveSpriteState : public IArrangeSpriteState
{
public:
	MoveSpriteState(SpriteSelection* selection);
	virtual ~MoveSpriteState();

	virtual	bool OnMouseMove(const Vector& pos);

private:
	std::vector<ISprite*> m_sprites;

	Vector m_center;
	std::vector<Vector> m_offset;

}; // MoveSpriteState

}

#endif // _DRAG2D_MOVE_SPRITE_STATE_H_