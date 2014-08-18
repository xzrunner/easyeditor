#ifndef _DRAG2D_SCALE_SPRITE_STATE_H_
#define _DRAG2D_SCALE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace d2d
{

class EditPanel;

class ScaleSpriteState : public IArrangeSpriteState
{
public:
	ScaleSpriteState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node);
	virtual ~ScaleSpriteState();

	virtual	bool OnMousePress(const Vector& pos);
	virtual AbstractAtomicOP* OnMouseRelease(const Vector& pos);

private:
	void Scale(const Vector& curr);

private:
	ISprite* m_sprite;

	SpriteCtrlNode::Node m_ctrl_node;

	Vector m_first_pos, m_first_scale;

}; // ScaleSpriteState

}

#endif // _DRAG2D_SCALE_SPRITE_STATE_H_