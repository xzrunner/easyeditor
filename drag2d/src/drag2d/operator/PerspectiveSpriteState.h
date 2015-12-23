#ifndef _DRAG2D_PERSPECTIVE_SPRITE_STATE_H_
#define _DRAG2D_PERSPECTIVE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace d2d
{

class PerspectiveSpriteState : public IArrangeSpriteState
{
public:
	PerspectiveSpriteState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node);
	virtual ~PerspectiveSpriteState();

	virtual void OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

private:
	void Perspective(const Vector& curr);

private:
	ISprite* m_sprite;

	SpriteCtrlNode::Node m_ctrl_node;

	Vector m_first_persp;

}; // PerspectiveSpriteState

}

#endif // _DRAG2D_PERSPECTIVE_SPRITE_STATE_H_