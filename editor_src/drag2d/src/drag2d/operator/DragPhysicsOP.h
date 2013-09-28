#pragma once

#include "ZoomViewOP.h"

#include <Box2D/Box2D.h>

namespace d2d
{
	class DragPhysicsOP : public ZoomViewOP
	{
	public:
		DragPhysicsOP(EditPanel* editPanel, b2World* world, b2Body* ground);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

	private:
		b2World* m_world;
		b2Body* m_ground;

	public:
		b2MouseJoint* m_mouseJoint;

		d2d::Vector currPos;

	}; // DragPhysicsOP
}

