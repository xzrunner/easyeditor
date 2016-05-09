#include "DragPhysicsOP.h"
#include "PhysicsQuery.h"
#include "physics_const.h"
#include "EditPanelImpl.h"

namespace ee
{

DragPhysicsOP::DragPhysicsOP(wxWindow* wnd, EditPanelImpl* stage, 
							 b2World* world, b2Body* ground)
	: ZoomViewOP(wnd, stage, true)
	, m_world(world)
	, m_ground(ground)
	, m_mouseJoint(NULL)
{
}

bool DragPhysicsOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (m_mouseJoint) return false;

	const sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	b2Body* body = PhysicsQuery::QueryOn(m_world, pos);
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

bool DragPhysicsOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (m_mouseJoint)
	{
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}

	return false;
}

bool DragPhysicsOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_mouseJoint)
	{
		m_curr_pos = m_stage->TransPosScrToProj(x, y);
		m_mouseJoint->SetTarget(b2Vec2(m_curr_pos.x / BOX2D_SCALE_FACTOR, m_curr_pos.y / BOX2D_SCALE_FACTOR));
	}

	return false;
}

}
