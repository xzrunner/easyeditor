#include "MotorJoint.h"
#include "Body.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>

#include <sprite2/S2_RVG.h>

namespace emodeling
{
	
MotorJoint::MotorJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_motorJoint)
	, m_max_force(1.0f)
	, m_max_torque(1.0f)
	, m_correction_factor(0.3f)
{
	m_linear_offset = b1->m_spr->GetPosition() - b0->m_spr->GetPosition();
	m_angular_offset = b1->m_spr->GetAngle() - b0->m_spr->GetAngle();
}

bool MotorJoint::IsContain(const sm::vec2& pos) const
{
	const sm::vec2 center = (m_body_a->m_spr->GetPosition() + m_body_b->m_spr->GetPosition()) * 0.5f;
	return sm::dis_pos_to_pos(center, pos) < JOINT_RADIUS_OUT;
}

bool MotorJoint::IsIntersect(const sm::rect& rect) const
{
	const sm::vec2 center = (m_body_a->m_spr->GetPosition() + m_body_b->m_spr->GetPosition()) * 0.5f;
	return sm::is_point_in_rect(center, rect);
}

void MotorJoint::Draw(DrawType type) const
{
	const sm::vec2 center = (m_body_a->m_spr->GetPosition() + m_body_b->m_spr->GetPosition()) * 0.5f;

	if (type == e_selected || type == e_mouseOn)
	{
		s2::RVG::SetColor(s2::Color(102, 204, 102));
		s2::RVG::DashLine(center, m_body_a->m_spr->GetPosition());
		s2::RVG::SetColor(s2::Color(102, 102, 204));
		s2::RVG::DashLine(center, m_body_b->m_spr->GetPosition());

		DrawBodyFlag();
	}

	DrawAnchor(center, type);
}

void MotorJoint::DrawAnchor(const sm::vec2& pos, DrawType type) const
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
	s2::RVG::SetColor(color);
	s2::RVG::Circle(pos, JOINT_RADIUS_OUT, false);
}

}