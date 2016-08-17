#ifndef _EASYEDITOR_SCALE_SPRITE_STATE_H_
#define _EASYEDITOR_SCALE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace ee
{

class ScaleSpriteState : public ArrangeSpriteState
{
public:
	ScaleSpriteState(Sprite* spr, const SpriteCtrlNode::Node& ctrl_node);
	virtual ~ScaleSpriteState();

	virtual void OnMouseRelease(const sm::vec2& pos);
	virtual	bool OnMouseDrag(const sm::vec2& pos);

protected:
	virtual void Scale(const sm::vec2& curr);

private:
	Sprite* m_spr;

	SpriteCtrlNode::Node m_ctrl_node;

	sm::vec2 m_first_pos, m_first_scale;

}; // ScaleSpriteState

}

#endif // _EASYEDITOR_SCALE_SPRITE_STATE_H_