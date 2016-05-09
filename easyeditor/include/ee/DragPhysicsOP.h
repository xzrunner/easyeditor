#ifndef _EASYEDITOR_DRAG_PHYSICS_OP_H_
#define _EASYEDITOR_DRAG_PHYSICS_OP_H_

#include "ZoomViewOP.h"

#include <SM_Vector.h>

#include <Box2D/Box2D.h>

namespace ee
{

class DragPhysicsOP : public ZoomViewOP
{
public:
	DragPhysicsOP(wxWindow* wnd, EditPanelImpl* stage, 
		b2World* world, b2Body* ground);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	b2World* m_world;
	b2Body* m_ground;

public:
	b2MouseJoint* m_mouseJoint;

	sm::vec2 m_curr_pos;

}; // DragPhysicsOP

}

#endif // _EASYEDITOR_DRAG_PHYSICS_OP_H_