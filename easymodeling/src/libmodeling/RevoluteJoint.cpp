#include "RevoluteJoint.h"
#include "Body.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/PrimitiveDraw.h>

namespace emodeling
{

RevoluteJoint::RevoluteJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_revoluteJoint)
	, m_reference_angle(0.0f)
	, m_enable_limit(false)
	, m_lower_angle(0.0f)
	, m_upper_angle(0.0f)
	, m_enable_motor(false)
	, m_max_motor_torque(0.0f)
	, m_motor_speed(0.0f)
{
	ee::Vector center = (b0->m_sprite->GetPosition() + b1->m_sprite->GetPosition()) * 0.5f;
	SetLocalAnchorA(center);
	SetLocalAnchorB(center);
}

bool RevoluteJoint::IsContain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool RevoluteJoint::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(GetWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(GetWorldAnchorB(), rect);
}

void RevoluteJoint::Draw(DrawType type) const
{
	const ee::Vector anchorA = GetWorldAnchorA(),
		anchorB = GetWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::PrimitiveDraw::DrawDashLine(anchorA, anchorB, ee::Colorf(1, 0, 0), 2);
		ee::PrimitiveDraw::DrawDashLine(anchorA, m_body_a->m_sprite->GetPosition(), ee::Colorf(0.4f, 0.8f, 0.4f), 2);
		ee::PrimitiveDraw::DrawDashLine(anchorB, m_body_b->m_sprite->GetPosition(), ee::Colorf(0.4f, 0.4f, 0.8f), 2);

		DrawBodyFlag();
	}

	DrawAnchor(anchorA, type);
	DrawAnchor(anchorB, type);
}

ee::Vector RevoluteJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_sprite);
}

ee::Vector RevoluteJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_sprite);
}

void RevoluteJoint::SetLocalAnchorA(const ee::Vector& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_sprite);
}

void RevoluteJoint::SetLocalAnchorB(const ee::Vector& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_sprite);
}

void RevoluteJoint::DrawAnchor(const ee::Vector& pos, DrawType type) const
{
	ee::Colorf color;
	switch (type)
	{
	case e_default:
		color.Set(0.8f, 0.8f, 0.8f);
		break;
	case e_mouseOn:
		color.Set(1, 1, 1);
		break;
	case e_selected:
		color.Set(1, 1, 0);
		break;
	}

	ee::PrimitiveDraw::DrawCircle(pos, JOINT_RADIUS_IN, true, 2, color);
	ee::PrimitiveDraw::DrawCircle(pos, JOINT_RADIUS_OUT, false, 2, color);
}

}