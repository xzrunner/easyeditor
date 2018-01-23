#ifndef _EASYEDITOR_OFFSET_SPRITE_STATE_H_
#define _EASYEDITOR_OFFSET_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "Sprite.h"

namespace s2 { class Camera; }

namespace ee
{

class OffsetSpriteState : public ArrangeSpriteState
{
public:
	OffsetSpriteState(const SprPtr& spr, const std::shared_ptr<s2::Camera>& camera);

	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual	bool OnMouseDrag(const sm::vec2& pos) override;

private:
	SprPtr m_spr;
	std::shared_ptr<s2::Camera> m_camera = nullptr;

	sm::vec2 m_old_offset;

}; // OffsetSpriteState

}

#endif // _EASYEDITOR_OFFSET_SPRITE_STATE_H_