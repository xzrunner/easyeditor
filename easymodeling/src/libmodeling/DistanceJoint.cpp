#include "DistanceJoint.h"
#include "Body.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>
#include <ee/Sprite.h>

namespace emodeling
{

DistanceJoint::DistanceJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_distanceJoint)
	, m_frequency_hz(0.0f)
	, m_damping_ratio(0.0f)
{
}

bool DistanceJoint::IsContain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool DistanceJoint::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(GetWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(GetWorldAnchorB(), rect);
}

void DistanceJoint::Draw(DrawType type) const
{
	const ee::Vector anchorA = GetWorldAnchorA(),
		anchorB = GetWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::RVG::DashLine(anchorA, m_body_a->m_sprite->GetPosition(), ee::Colorf(0.4f, 0.8f, 0.4f), 2);
		ee::RVG::DashLine(anchorB, m_body_b->m_sprite->GetPosition(), ee::Colorf(0.4f, 0.4f, 0.8f), 2);

		DrawBodyFlag();
	}

	DrawAnchor(anchorA, type);
	DrawAnchor(anchorB, type);
	DrawConnection(anchorA, anchorB, type);
}


ee::Vector DistanceJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_sprite);
}

ee::Vector DistanceJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_sprite);
}

void DistanceJoint::SetLocalAnchorA(const ee::Vector& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_sprite);
}

void DistanceJoint::SetLocalAnchorB(const ee::Vector& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_sprite);
}

void DistanceJoint::DrawAnchor(const ee::Vector& pos, DrawType type) const
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

	ee::RVG::Circle(pos, JOINT_RADIUS_IN, true, color);
	ee::RVG::Circle(pos, JOINT_RADIUS_OUT, false, color);
}

void DistanceJoint::DrawConnection(const ee::Vector& worldAnchorA, 
								   const ee::Vector& worldAnchorB, DrawType type) const
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

	ee::RVG::Line(worldAnchorA, worldAnchorB, color, 1);
}

}