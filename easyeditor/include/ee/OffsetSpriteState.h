#ifndef _EASYEDITOR_OFFSET_SPRITE_STATE_H_
#define _EASYEDITOR_OFFSET_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Vector.h"

namespace ee
{

class Sprite;

class OffsetSpriteState : public ArrangeSpriteState
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

#endif // _EASYEDITOR_OFFSET_SPRITE_STATE_H_