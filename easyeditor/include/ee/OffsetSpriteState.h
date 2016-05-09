#ifndef _EASYEDITOR_OFFSET_SPRITE_STATE_H_
#define _EASYEDITOR_OFFSET_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"

namespace ee
{

class Sprite;

class OffsetSpriteState : public ArrangeSpriteState
{
public:
	OffsetSpriteState(Sprite* sprite);
	virtual ~OffsetSpriteState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

private:
	Sprite* m_sprite;

	sm::vec2 m_old_offset;

}; // OffsetSpriteState

}

#endif // _EASYEDITOR_OFFSET_SPRITE_STATE_H_