#ifndef _DRAG2D_OFFSET_SPRITE_STATE_H_
#define _DRAG2D_OFFSET_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"

#include "common/Vector.h"

namespace d2d
{

class ISprite;

class OffsetSpriteState : public IArrangeSpriteState
{
public:
	OffsetSpriteState(ISprite* sprite);
	virtual ~OffsetSpriteState();

	virtual	bool OnMousePress(const Vector& pos);
	virtual AbstractAtomicOP* OnMouseRelease(const Vector& pos);

private:
	ISprite* m_sprite;

	Vector m_old_offset;

}; // OffsetSpriteState

}

#endif // _DRAG2D_OFFSET_SPRITE_STATE_H_