#include "PrismaticJoint.h"
#include "Body.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace emodeling
{

PrismaticJoint::PrismaticJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_prismaticJoint)
	, m_local_axis_a(1, 0)
	, m_reference_angle(0.0f)
	, m_enable_limit(false)
	, m_lower_translation(0.0f)
	, m_upper_translation(0.0f)
	, m_enable_motor(false)
	, m_max_motor_force(0.0f)
	, m_motor_speed(0.0f)
{
	ee::Vector center = (b0->m_sprite->GetPosition() + b1->m_sprite->GetPosition()) * 0.5f;
	SetLocalAnchorA(center);
	SetLocalAnchorB(center);
}

bool PrismaticJoint::IsContain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool PrismaticJoint::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(GetWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(GetWorldAnchorB(), rect);
}

void PrismaticJoint::Draw(DrawType type) const
{
	const ee::Vector anchorA = GetWorldAnchorA(),
		anchorB = GetWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::RVG::Color(ee::Colorf(1, 0, 0));
		ee::RVG::DashLine(anchorA, anchorB);
		ee::RVG::Color(ee::Colorf(0.4f, 0.8f, 0.4f));
		ee::RVG::DashLine(anchorA, m_body_a->m_sprite->GetPosition());
		ee::RVG::Color(ee::Colorf(0.4f, 0.4f, 0.8f));
		ee::RVG::DashLine(anchorB, m_body_b->m_sprite->GetPosition());

		DrawBodyFlag();
	}

	DrawAnchor(anchorA, type);
	DrawAnchor(anchorB, type);
}

ee::Vector PrismaticJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_sprite);
}

ee::Vector PrismaticJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_sprite);
}

void PrismaticJoint::SetLocalAnchorA(const ee::Vector& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_sprite);
}

void PrismaticJoint::SetLocalAnchorB(const ee::Vector& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_sprite);
}

void PrismaticJoint::DrawAnchor(const ee::Vector& pos, DrawType type) const
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

	ee::RVG::Color(color);
	ee::RVG::Circle(pos, JOINT_RADIUS_IN, true);
	ee::RVG::Circle(pos, JOINT_RADIUS_OUT, false);
}

}