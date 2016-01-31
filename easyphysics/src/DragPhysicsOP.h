#ifndef _EPHYSICS_DRAG_PHYSICS_OP_H_
#define _EPHYSICS_DRAG_PHYSICS_OP_H_


#include <Box2D/Box2D.h>

namespace ephysics
{

class DragPhysicsOP : public ee::ZoomViewOP
{
public:
	DragPhysicsOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		b2World* world, b2Body* ground);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	b2World* m_world;
	b2Body* m_ground;

public:
	b2MouseJoint* m_mouseJoint;

	ee::Vector currPos;

}; // DragPhysicsOP

}

#endif // _EPHYSICS_DRAG_PHYSICS_OP_H_