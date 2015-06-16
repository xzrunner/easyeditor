#ifndef _EPHYSICS_DRAG_PHYSICS_OP_H_
#define _EPHYSICS_DRAG_PHYSICS_OP_H_

#include <drag2d.h>
#include <Box2D/Box2D.h>

namespace ephysics
{

class DragPhysicsOP : public d2d::ZoomViewOP
{
public:
	DragPhysicsOP(d2d::EditPanel* editPanel, b2World* world, b2Body* ground);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	b2World* m_world;
	b2Body* m_ground;

public:
	b2MouseJoint* m_mouseJoint;

	d2d::Vector currPos;

}; // DragPhysicsOP

}

#endif // _EPHYSICS_DRAG_PHYSICS_OP_H_