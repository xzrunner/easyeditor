
#include "MotorJoint.h"
#include "Body.h"

using namespace libmodeling;

MotorJoint::MotorJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_motorJoint)
	, maxForce(1.0f)
	, maxTorque(1.0f)
	, correctionFactor(0.3f)
{
	linearOffset = b1->sprite->GetPosition() - b0->sprite->GetPosition();
	angularOffset = b1->sprite->GetAngle() - b0->sprite->GetAngle();
}

bool MotorJoint::isContain(const ee::Vector& pos) const
{
	const ee::Vector center = (bodyA->sprite->GetPosition() + bodyB->sprite->GetPosition()) * 0.5f;
	return ee::Math2D::GetDistance(center, pos) < JOINT_RADIUS_OUT;
}

bool MotorJoint::isIntersect(const ee::Rect& rect) const
{
	const ee::Vector center = (bodyA->sprite->GetPosition() + bodyB->sprite->GetPosition()) * 0.5f;
	return ee::Math2D::IsPointInRect(center, rect);
}

void MotorJoint::draw(DrawType type) const
{
	const ee::Vector center = (bodyA->sprite->GetPosition() + bodyB->sprite->GetPosition()) * 0.5f;

	if (type == e_selected || type == e_mouseOn)
	{
		ee::PrimitiveDraw::DrawDashLine(center, bodyA->sprite->GetPosition(), ee::Colorf(0.4f, 0.8f, 0.4f), 2);
		ee::PrimitiveDraw::DrawDashLine(center, bodyB->sprite->GetPosition(), ee::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag();
	}

	drawAnchor(center, type);
}

void MotorJoint::drawAnchor(const ee::Vector& pos, DrawType type) const
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
