#include "MotorJoint.h"
#include "Body.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/PrimitiveDraw.h>

namespace emodeling
{
	
MotorJoint::MotorJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_motorJoint)
	, m_max_force(1.0f)
	, m_max_torque(1.0f)
	, m_correction_factor(0.3f)
{
	m_linear_offset = b1->m_sprite->GetPosition() - b0->m_sprite->GetPosition();
	m_angular_offset = b1->m_sprite->GetAngle() - b0->m_sprite->GetAngle();
}

bool MotorJoint::IsContain(const ee::Vector& pos) const
{
	const ee::Vector center = (m_body_a->m_sprite->GetPosition() + m_body_b->m_sprite->GetPosition()) * 0.5f;
	return ee::Math2D::GetDistance(center, pos) < JOINT_RADIUS_OUT;
}

bool MotorJoint::IsIntersect(const ee::Rect& rect) const
{
	const ee::Vector center = (m_body_a->m_sprite->GetPosition() + m_body_b->m_sprite->GetPosition()) * 0.5f;
	return ee::Math2D::IsPointInRect(center, rect);
}

void MotorJoint::Draw(DrawType type) const
{
	const ee::Vector center = (m_body_a->m_sprite->GetPosition() + m_body_b->m_sprite->GetPosition()) * 0.5f;

	if (type == e_selected || type == e_mouseOn)
	{
		ee::PrimitiveDraw::DrawDashLine(center, m_body_a->m_sprite->GetPosition(), ee::Colorf(0.4f, 0.8f, 0.4f), 2);
		ee::PrimitiveDraw::DrawDashLine(center, m_body_b->m_sprite->GetPosition(), ee::Colorf(0.4f, 0.4f, 0.8f), 2);

		DrawBodyFlag();
	}

	DrawAnchor(center, type);
}

void MotorJoint::DrawAnchor(const ee::Vector& pos, DrawType type) const
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