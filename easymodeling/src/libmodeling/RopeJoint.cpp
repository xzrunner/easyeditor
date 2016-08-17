#include "RopeJoint.h"
#include "Body.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>

#include <sprite2/S2_RVG.h>

namespace emodeling
{

RopeJoint::RopeJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_ropeJoint)
	, m_max_length(0.0f)
{
	sm::vec2 center = (b0->m_spr->GetPosition() + b1->m_spr->GetPosition()) * 0.5f;
	SetLocalAnchorA(center);
	SetLocalAnchorB(center);
}

bool RopeJoint::IsContain(const sm::vec2& pos) const
{
	return ee::Math2D::GetDistance(GetWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(GetWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool RopeJoint::IsIntersect(const sm::rect& rect) const
{
	return ee::Math2D::IsPointInRect(GetWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(GetWorldAnchorB(), rect);
}

void RopeJoint::Draw(DrawType type) const
{
	const sm::vec2 anchorA = GetWorldAnchorA(),
		anchorB = GetWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		s2::RVG::SetColor(s2::Color(255, 0, 0));
		s2::RVG::DashLine(anchorA, anchorB);
		s2::RVG::SetColor(s2::Color(102, 204, 102));
		s2::RVG::DashLine(anchorA, m_body_a->m_spr->GetPosition());
		s2::RVG::SetColor(s2::Color(102, 102, 204));
		s2::RVG::DashLine(anchorB, m_body_b->m_spr->GetPosition());

		DrawBodyFlag();
	}

	DrawAnchor(anchorA, type);
	DrawAnchor(anchorB, type);
}

sm::vec2 RopeJoint::GetWorldAnchorA() const
{
	return TransLocalToWorld(m_local_anchor_a, m_body_a->m_spr);
}

sm::vec2 RopeJoint::GetWorldAnchorB() const
{
	return TransLocalToWorld(m_local_anchor_b, m_body_b->m_spr);
}

void RopeJoint::SetLocalAnchorA(const sm::vec2& world)
{
	m_local_anchor_a = TransWorldToLocal(world, m_body_a->m_spr);
}

void RopeJoint::SetLocalAnchorB(const sm::vec2& world)
{
	m_local_anchor_b = TransWorldToLocal(world, m_body_b->m_spr);
}

void RopeJoint::DrawAnchor(const sm::vec2& pos, DrawType type) const
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

}