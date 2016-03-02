#include "WheelJoint.h"
#include "Body.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>
#include <ee/Sprite.h>

namespace emodeling
{

WheelJoint::WheelJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_wheelJoint)
	, m_local_axis_a(1, 0)
	, m_enable_motor(false)
	, m_max_motor_torque(0.0f)
	, m_motor_speed(0.0f)
	, m_frequency_hz(2.0f)
	, m_damping_ratio(0.7f)
{
}

bool WheelJoint::IsContain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool WheelJoint::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(GetWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(GetWorldAnchorB(), rect);
}

void WheelJoint::Draw(DrawType type) const
{
	const ee::Vector anchorA = GetWorldAnchorA(),
		anchorB = GetWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::RVG::Color(ee::Colorf(0.4f, 0.8f, 0.4f));
		ee::RVG::DashLine(anchorA, m_body_a->m_sprite->GetPosition());
		ee::RVG::Color(ee::Colorf(0.4f, 0.4f, 0.8f));
		ee::RVG::DashLine(anchorB, m_body_b->m_sprite->GetPosition());

		DrawAxisALine(anchorA);
		DrawFootBLine(anchorA, anchorB);
		DrawBodyFlag();
	}

	DrawAnchorA(anchorA, type);
	DrawAnchorB(anchorB, type);
}

ee::Vector WheelJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_sprite);
}

ee::Vector WheelJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_sprite);
}

void WheelJoint::SetLocalAnchorA(const ee::Vector& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_sprite);
}

void WheelJoint::SetLocalAnchorB(const ee::Vector& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_sprite);
}

void WheelJoint::DrawAxisALine(const ee::Vector& worldAnchorA) const
{
	ee::Vector unit = m_local_axis_a;
	unit.Normalize();

	const ee::Vector start = worldAnchorA + unit * 150,
		end = worldAnchorA - unit * 150;

	ee::RVG::Color(ee::Colorf(0.4f, 0.6f, 0.4f));
	ee::RVG::LineWidth(1);
	ee::RVG::DotDashLine(start, end);
	ee::RVG::LineWidth(2);	
}

void WheelJoint::DrawFootBLine(const ee::Vector& worldAnchorA, const ee::Vector& worldAnchorB) const
{
	ee::Vector otherA = worldAnchorA + m_local_axis_a;

	ee::Vector foot;
	ee::Math2D::GetFootOfPerpendicular(worldAnchorA, otherA, worldAnchorB, &foot);

	ee::RVG::Color(ee::Colorf(1, 0, 0));
	ee::RVG::LineWidth(1);
	ee::RVG::DashLine(worldAnchorB, foot);
	ee::RVG::LineWidth(2);	

	ee::RVG::Circle(foot, JOINT_RADIUS_IN, true);
}

void WheelJoint::DrawAnchorA(const ee::Vector& pos, DrawType type) const
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
	ee::RVG::Rect(pos, JOINT_RADIUS_OUT, JOINT_RADIUS_OUT, true);
}

void WheelJoint::DrawAnchorB(const ee::Vector& pos, DrawType type) const
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