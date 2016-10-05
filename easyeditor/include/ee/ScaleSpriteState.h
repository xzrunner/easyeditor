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

	virtual void SetScaleTimes(const sm::vec2& st);

protected:
	Sprite* m_spr;

private:
	SpriteCtrlNode::Node m_ctrl_node;

	sm::vec2 m_first_pos, m_first_scale;

}; // ScaleSpriteState

}

#endif // _EASYEDITOR_SCALE_SPRITE_STATE_H_