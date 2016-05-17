#include "RevoluteJoint.h"
#include "Body.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

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
	sm::vec2 center = (b0->m_sprite->GetPosition() + b1->m_sprite->GetPosition()) * 0.5f;
	SetLocalAnchorA(center);
	SetLocalAnchorB(center);
}

bool RevoluteJoint::IsContain(const sm::vec2& pos) const
{
	return ee::Math2D::GetDistance(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool RevoluteJoint::IsIntersect(const sm::rect& rect) const
{
	return ee::Math2D::IsPointInRect(GetWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(GetWorldAnchorB(), rect);
}

void RevoluteJoint::Draw(DrawType type) const
{
	const sm::vec2 anchorA = GetWorldAnchorA(),
		anchorB = GetWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::RVG::Color(s2::Color(255, 0, 0));
		ee::RVG::DashLine(anchorA, anchorB);
		ee::RVG::Color(s2::Color(102, 204, 102));
		ee::RVG::DashLine(anchorA, m_body_a->m_sprite->GetPosition());
		ee::RVG::Color(s2::Color(102, 102, 204));
		ee::RVG::DashLine(anchorB, m_body_b->m_sprite->GetPosition());

		DrawBodyFlag();
	}

	DrawAnchor(anchorA, type);
	DrawAnchor(anchorB, type);
}

sm::vec2 RevoluteJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_sprite);
}

sm::vec2 RevoluteJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_sprite);
}

void RevoluteJoint::SetLocalAnchorA(const sm::vec2& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_sprite);
}

void RevoluteJoint::SetLocalAnchorB(const sm::vec2& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_sprite);
}

void RevoluteJoint::DrawAnchor(const sm::vec2& pos, DrawType type) const
{
	s2::Color color;
	switch (type)
	{
	case e_default:
		color.FromFloat(0.8f, 0.8f, 0.8f);
		break;
	case e_mouseOn:
		color.FromFloat(1, 1, 1);
		break;
	case e_selected:
		color.FromFloat(1, 1, 0);
		break;
	}

	ee::RVG::Color(color);
	ee::RVG::Circle(pos, JOINT_RADIUS_IN, true);
	ee::RVG::Circle(pos, JOINT_RADIUS_OUT, false);
}

}