#include "DragPhysicsOP.h"
#include "PhysicsQuery.h"
#include "const.h"

namespace ephysics
{

DragPhysicsOP::DragPhysicsOP(d2d::EditPanel* editPanel, b2World* world, b2Body* ground)
	: d2d::ZoomViewOP(editPanel, true)
	, m_world(world)
	, m_ground(ground)
	, m_mouseJoint(NULL)
{
	currPos.setInvalid();
}

bool DragPhysicsOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	if (m_mouseJoint) return false;

	const d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	b2Body* body = PhysicsQuery::queryOn(m_world, pos);
	if (body && body != m_ground)
	{
		b2MouseJointDef md;
		md.bodyA = m_ground;
		md.bodyB = body;
		md.target = b2Vec2(pos.x / BOX2D_SCALE_FACTOR, pos.y / BOX2D_SCALE_FACTOR);
		md.maxForce = 1000.0f * body->GetMass();
		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
		body->SetAwake(true);
	}

	return false;
}

bool DragPhysicsOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (m_mouseJoint)
	{
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
		currPos.setInvalid();
	}

	return false;
}

bool DragPhysicsOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_mouseJoint)
	{
		currPos = m_editPanel->transPosScreenToProject(x, y);
		m_mouseJoint->SetTarget(b2Vec2(currPos.x / BOX2D_SCALE_FACTOR, currPos.y / BOX2D_SCALE_FACTOR));
	}

	return false;
}

}
