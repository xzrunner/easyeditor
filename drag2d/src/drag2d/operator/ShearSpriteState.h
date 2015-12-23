#ifndef _DRAG2D_SHEAR_SPRITE_STATE_H_
#define _DRAG2D_SHEAR_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace d2d
{

class ShearSpriteState : public IArrangeSpriteState
{
public:
	ShearSpriteState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node);
	virtual ~ShearSpriteState();

	virtual void OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

private:
	void Shear(const Vector& curr);
	void Shear2(const Vector& curr);

private:
	ISprite* m_sprite;

	SpriteCtrlNode::Node m_ctrl_node;

	Vector m_first_shear;

}; // ShearSpriteState

}

#endif // _DRAG2D_SHEAR_SPRITE_STATE_H_