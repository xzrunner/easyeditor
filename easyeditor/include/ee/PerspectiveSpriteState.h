#ifndef _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_
#define _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace ee
{

class PerspectiveSpriteState : public ArrangeSpriteState
{
public:
	PerspectiveSpriteState(Sprite* spr, const SpriteCtrlNode::Node& ctrl_node);
	virtual ~PerspectiveSpriteState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

private:
	void Perspective(const sm::vec2& curr);

private:
	Sprite* m_spr;

	SpriteCtrlNode::Node m_ctrl_node;

	sm::vec2 m_first_persp;

}; // PerspectiveSpriteState

}

#endif // _EASYEDITOR_PERSPECTIVE_SPRITE_STATE_H_