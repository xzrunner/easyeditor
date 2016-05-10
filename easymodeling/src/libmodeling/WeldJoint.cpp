#include "WeldJoint.h"
#include "Body.h"

#include <ee/Math2D.h>
#include <ee/EE_RVG.h>
#include <ee/Sprite.h>

namespace emodeling
{

WeldJoint::WeldJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_weldJoint)
	, m_reference_angle(0.0f)
	, m_frequency_hz(0.0f)
	, m_damping_ratio(0.0f)
{
	sm::vec2 center = (b0->m_sprite->GetPosition() + b1->m_sprite->GetPosition()) * 0.5f;
	SetLocalAnchorA(center);
	SetLocalAnchorB(center);
}

bool WeldJoint::IsContain(const sm::vec2& pos) const
{
	return ee::Math2D::GetDistance(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool WeldJoint::IsIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(GetWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(GetWorldAnchorB(), rect);
}

void WeldJoint::Draw(DrawType type) const
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

sm::vec2 WeldJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_sprite);
}

sm::vec2 WeldJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_sprite);
}

void WeldJoint::SetLocalAnchorA(const sm::vec2& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_sprite);
}

void WeldJoint::SetLocalAnchorB(const sm::vec2& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_sprite);
}

void WeldJoint::DrawAnchor(const sm::vec2& pos, DrawType type) const
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