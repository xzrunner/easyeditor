
#include "WheelJoint.h"
#include "Body.h"

using namespace libmodeling;

WheelJoint::WheelJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_wheelJoint)
	, localAxisA(1, 0)
	, enableMotor(false)
	, maxMotorTorque(0.0f)
	, motorSpeed(0.0f)
	, frequencyHz(2.0f)
	, dampingRatio(0.7f)
{
}

bool WheelJoint::isContain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool WheelJoint::isIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(getWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(getWorldAnchorB(), rect);
}

void WheelJoint::draw(DrawType type) const
{
	const ee::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::PrimitiveDraw::DrawDashLine(anchorA, bodyA->sprite->GetPosition(), ee::Colorf(0.4f, 0.8f, 0.4f), 2);
		ee::PrimitiveDraw::DrawDashLine(anchorB, bodyB->sprite->GetPosition(), ee::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawAxisALine(anchorA);
		drawFootBLine(anchorA, anchorB);
		drawBodyFlag();
	}

	drawAnchorA(anchorA, type);
	drawAnchorB(anchorB, type);
}

ee::Vector WheelJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

ee::Vector WheelJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void WheelJoint::setLocalAnchorA(const ee::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void WheelJoint::setLocalAnchorB(const ee::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void WheelJoint::drawAxisALine(const ee::Vector& worldAnchorA) const
{
	ee::Vector unit = localAxisA;
	unit.Normalize();

	const ee::Vector start = worldAnchorA + unit * 150,
		end = worldAnchorA - unit * 150;

	ee::PrimitiveDraw::DrawDotDashLine(start, end, ee::Colorf(0.4f, 0.6f, 0.4f), 1);
}

void WheelJoint::drawFootBLine(const ee::Vector& worldAnchorA, const ee::Vector& worldAnchorB) const
{
	ee::Vector otherA = worldAnchorA + localAxisA;

	ee::Vector foot;
	ee::Math2D::GetFootOfPerpendicular(worldAnchorA, otherA, worldAnchorB, &foot);

	ee::PrimitiveDraw::DrawDashLine(worldAnchorB, foot, ee::Colorf(1, 0, 0), 1);
	ee::PrimitiveDraw::DrawCircle(foot, JOINT_RADIUS_IN, true, 2, ee::Colorf(1, 0, 0));
}

void WheelJoint::drawAnchorA(const ee::Vector& pos, DrawType type) const
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
	ee::PrimitiveDraw::DrawRect(pos, JOINT_RADIUS_OUT, ee::ShapeStyle(true, color));
}

void WheelJoint::drawAnchorB(const ee::Vector& pos, DrawType type) const
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
