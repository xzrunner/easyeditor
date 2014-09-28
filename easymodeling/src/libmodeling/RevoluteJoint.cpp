
#include "RevoluteJoint.h"
#include "Body.h"

using namespace libmodeling;

RevoluteJoint::RevoluteJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_revoluteJoint)
	, referenceAngle(0.0f)
	, enableLimit(false)
	, lowerAngle(0.0f)
	, upperAngle(0.0f)
	, enableMotor(false)
	, maxMotorTorque(0.0f)
	, motorSpeed(0.0f)
{
	d2d::Vector center = (b0->sprite->getPosition() + b1->sprite->getPosition()) * 0.5f;
	setLocalAnchorA(center);
	setLocalAnchorB(center);
}

bool RevoluteJoint::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::getDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| d2d::Math::getDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool RevoluteJoint::isIntersect(const d2d::Rect& rect) const
{
	return d2d::Math::isPointInRect(getWorldAnchorA(), rect) 
		|| d2d::Math::isPointInRect(getWorldAnchorB(), rect);
}

void RevoluteJoint::draw(const d2d::Screen& scr, DrawType type) const
{
	const d2d::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		d2d::PrimitiveDraw::drawDashLine(scr, anchorA, anchorB, d2d::Colorf(1, 0, 0), 2);
		d2d::PrimitiveDraw::drawDashLine(scr, anchorA, bodyA->sprite->getPosition(), d2d::Colorf(0.4f, 0.8f, 0.4f), 2);
		d2d::PrimitiveDraw::drawDashLine(scr, anchorB, bodyB->sprite->getPosition(), d2d::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag(scr);
	}

	drawAnchor(scr, anchorA, type);
	drawAnchor(scr, anchorB, type);
}

d2d::Vector RevoluteJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

d2d::Vector RevoluteJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void RevoluteJoint::setLocalAnchorA(const d2d::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void RevoluteJoint::setLocalAnchorB(const d2d::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void RevoluteJoint::drawAnchor(const d2d::Screen& scr, const d2d::Vector& pos, DrawType type) const
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
