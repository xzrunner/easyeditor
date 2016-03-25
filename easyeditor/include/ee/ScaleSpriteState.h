#ifndef _EASYEDITOR_SCALE_SPRITE_STATE_H_
#define _EASYEDITOR_SCALE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"
#include "SpriteCtrlNode.h"

namespace ee
{

class EditPanelImpl;

class ScaleSpriteState : public ArrangeSpriteState
{
public:
	ScaleSpriteState(EditPanelImpl* stage, Sprite* sprite, 
		const SpriteCtrlNode::Node& ctrl_node);
	virtual ~ScaleSpriteState();

	virtual void OnMouseRelease(const Vector& pos);
	virtual	bool OnMouseDrag(const Vector& pos);

protected:
	virtual void Scale(const Vector& curr);

private:
	EditPanelImpl* m_stage;

	Sprite* m_sprite;

	SpriteCtrlNode::Node m_ctrl_node;

	Vector m_first_pos, m_first_scale;

}; // ScaleSpriteState

}

#endif // _EASYEDITOR_SCALE_SPRITE_STATE_H_