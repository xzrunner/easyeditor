#include "DistanceJoint.h"
#include "Body.h"

#include <ee/Math2D.h>
#include <ee/Sprite.h>

#include <sprite2/S2_RVG.h>

namespace emodeling
{

DistanceJoint::DistanceJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_distanceJoint)
	, m_frequency_hz(0.0f)
	, m_damping_ratio(0.0f)
{
}

bool DistanceJoint::IsContain(const sm::vec2& pos) const
{
	return sm::dis_pos_to_pos(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| sm::dis_pos_to_pos(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool DistanceJoint::IsIntersect(const sm::rect& rect) const
{
	return sm::is_point_in_rect(GetWorldAnchorA(), rect) 
		|| sm::is_point_in_rect(GetWorldAnchorB(), rect);
}

void DistanceJoint::Draw(DrawType type) const
{
	const sm::vec2 anchorA = GetWorldAnchorA(),
		anchorB = GetWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		s2::RVG::SetColor(s2::Color(102, 204, 102));
		s2::RVG::DashLine(anchorA, m_body_a->m_spr->GetPosition());
		s2::RVG::SetColor(s2::Color(102, 102, 204));
		s2::RVG::DashLine(anchorB, m_body_b->m_spr->GetPosition());

		DrawBodyFlag();
	}

	DrawAnchor(anchorA, type);
	DrawAnchor(anchorB, type);
	DrawConnection(anchorA, anchorB, type);
}


sm::vec2 DistanceJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_spr);
}

sm::vec2 DistanceJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_spr);
}

void DistanceJoint::SetLocalAnchorA(const sm::vec2& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_spr);
}

void DistanceJoint::SetLocalAnchorB(const sm::vec2& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_spr);
}

void DistanceJoint::DrawAnchor(const sm::vec2& pos, DrawType type) const
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

	s2::RVG::SetColor(color);
	s2::RVG::Circle(pos, JOINT_RADIUS_IN, true);
	s2::RVG::Circle(pos, JOINT_RADIUS_OUT, false);
}

void DistanceJoint::DrawConnection(const sm::vec2& worldAnchorA, 
								   const sm::vec2& worldAnchorB, DrawType type) const
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

	s2::RVG::SetColor(color);
	s2::RVG::LineWidth(1);
	s2::RVG::Line(worldAnchorA, worldAnchorB);
}

}