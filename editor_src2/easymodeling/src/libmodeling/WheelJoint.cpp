
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

bool WheelJoint::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::getDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| d2d::Math::getDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool WheelJoint::isIntersect(const d2d::Rect& rect) const
{
	return d2d::Math::isPointInRect(getWorldAnchorA(), rect) 
		|| d2d::Math::isPointInRect(getWorldAnchorB(), rect);
}

void WheelJoint::draw(DrawType type) const
{
	const d2d::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		d2d::PrimitiveDraw::drawDashLine(anchorA, bodyA->sprite->getPosition(), d2d::Colorf(0.4f, 0.8f, 0.4f), 2);
		d2d::PrimitiveDraw::drawDashLine(anchorB, bodyB->sprite->getPosition(), d2d::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawAxisALine(anchorA);
		drawFootBLine(anchorA, anchorB);
		drawBodyFlag();
	}

	drawAnchorA(anchorA, type);
	drawAnchorB(anchorB, type);
}

d2d::Vector WheelJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

d2d::Vector WheelJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void WheelJoint::setLocalAnchorA(const d2d::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void WheelJoint::setLocalAnchorB(const d2d::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void WheelJoint::drawAxisALine(const d2d::Vector& worldAnchorA) const
{
	d2d::Vector unit = localAxisA;
	unit.normalize();

	const d2d::Vector start = worldAnchorA + unit * 150,
		end = worldAnchorA - unit * 150;

	d2d::PrimitiveDraw::drawDotDashLine(start, end, d2d::Colorf(0.4f, 0.6f, 0.4f), 1);
}

void WheelJoint::drawFootBLine(const d2d::Vector& worldAnchorA, const d2d::Vector& worldAnchorB) const
{
	d2d::Vector otherA = worldAnchorA + localAxisA;

	d2d::Vector foot;
	d2d::Math::getFootOfPerpendicular(worldAnchorA, otherA, worldAnchorB, &foot);

	d2d::PrimitiveDraw::drawDashLine(worldAnchorB, foot, d2d::Colorf(1, 0, 0), 1);
	d2d::PrimitiveDraw::drawCircle(foot, JOINT_RADIUS_IN, true, 2, d2d::Colorf(1, 0, 0));
}

void WheelJoint::drawAnchorA(const d2d::Vector& pos, DrawType type) const
{
	d2d::Colorf color;
	switch (type)
	{
	case e_default:
		color.set(0.8f, 0.8f, 0.8f);
		break;
	case e_mouseOn:
		color.set(1, 1, 1);
		break;
	case e_selected:
		color.set(1, 1, 0);
		break;
	}

	d2d::PrimitiveDraw::drawCircle(pos, JOINT_RADIUS_IN, true, 2, color);
	d2d::PrimitiveDraw::rect(pos, JOINT_RADIUS_OUT, d2d::ShapeStyle(true, color));
}

void WheelJoint::drawAnchorB(const d2d::Vector& pos, DrawType type) const
{
	d2d::Colorf color;
	switch (type)
	{
	case e_default:
		color.set(0.8f, 0.8f, 0.8f);
		break;
	case e_mouseOn:
		color.set(1, 1, 1);
		break;
	case e_selected:
		color.set(1, 1, 0);
		break;
	}

	d2d::PrimitiveDraw::drawCircle(pos, JOINT_RADIUS_IN, true, 2, color);
	d2d::PrimitiveDraw::drawCircle(pos, JOINT_RADIUS_OUT, false, 2, color);
}
