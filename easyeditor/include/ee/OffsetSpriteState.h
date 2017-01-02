#ifndef _EASYEDITOR_OFFSET_SPRITE_STATE_H_
#define _EASYEDITOR_OFFSET_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"

namespace gum { class Camera; }

namespace ee
{

class Sprite;

class OffsetSpriteState : public ArrangeSpriteState
{
public:
	OffsetSpriteState(Sprite* spr, const gum::Camera* camera);
	virtual ~OffsetSpriteState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

private:
	Sprite* m_spr;
	const gum::Camera* m_camera;

	sm::vec2 m_old_offset;

}; // OffsetSpriteState

}

#endif // _EASYEDITOR_OFFSET_SPRITE_STATE_H_