#ifndef _DRAG2D_OFFSET_SPRITE_STATE_H_
#define _DRAG2D_OFFSET_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"

#include "common/Vector.h"

namespace d2d
{

class Sprite;

class OffsetSpriteState : public IArrangeSpriteState
{
public:
	OffsetSpriteState(Sprite* sprite);
	virtual ~OffsetSpriteState();

	virtual void OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

private:
	Sprite* m_sprite;

	Vector m_old_offset;

}; // OffsetSpriteState

}

#endif // _DRAG2D_OFFSET_SPRITE_STATE_H_