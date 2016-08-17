#ifndef _EASYEDITOR_SHEAR_SPRITE_STATE_H_
#define _EASYEDITOR_SHEAR_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace ee
{

class ShearSpriteState : public ArrangeSpriteState
{
public:
	ShearSpriteState(Sprite* spr, const SpriteCtrlNode::Node& ctrl_node);
	virtual ~ShearSpriteState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

private:
	void Shear(const sm::vec2& curr);
	void Shear2(const sm::vec2& curr);

private:
	Sprite* m_spr;

	SpriteCtrlNode::Node m_ctrl_node;

	sm::vec2 m_first_shear;

}; // ShearSpriteState

}

#endif // _EASYEDITOR_SHEAR_SPRITE_STATE_H_