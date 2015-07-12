#ifndef _DRAG2D_MOVE_SPRITE_STATE_H_
#define _DRAG2D_MOVE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"

#include "common/Vector.h"

#include <vector>

namespace d2d
{

class ISprite;
class SpriteSelection;

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