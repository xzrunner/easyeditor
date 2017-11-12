#ifndef _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_
#define _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "SpriteCtrlNode.h"
#include "Sprite.h"

namespace ee
{

class PerspectiveSpriteState : public ArrangeSpriteState
{
public:
	PerspectiveSpriteState(const SprPtr& spr, const SpriteCtrlNode::Node& ctrl_node);

	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual	bool OnMouseDrag(const sm::vec2& pos) override;

private:
	void Perspective(const sm::vec2& curr);

private:
	SprPtr m_spr;

	SpriteCtrlNode::Node m_ctrl_node;

	sm::vec2 m_first_persp;

}; // PerspectiveSpriteState

}

#endif // _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_