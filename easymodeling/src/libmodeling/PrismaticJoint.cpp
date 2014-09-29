
#include "PrismaticJoint.h"
#include "Body.h"

using namespace libmodeling;

PrismaticJoint::PrismaticJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_prismaticJoint)
	, localAxisA(1, 0)
	, referenceAngle(0.0f)
	, enableLimit(false)
	, lowerTranslation(0.0f)
	, upperTranslation(0.0f)
	, enableMotor(false)
	, maxMotorForce(0.0f)
	, motorSpeed(0.0f)
{
	d2d::Vector center = (b0->sprite->getPosition() + b1->sprite->getPosition()) * 0.5f;
	setLocalAnchorA(center);
	setLocalAnchorB(center);
}

bool PrismaticJoint::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::getDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| d2d::Math::getDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool PrismaticJoint::isIntersect(const d2d::Rect& rect) const
{
	return d2d::Math::isPointInRect(getWorldAnchorA(), rect) 
		|| d2d::Math::isPointInRect(getWorldAnchorB(), rect);
}

void PrismaticJoint::draw(DrawType type) const
{
	const d2d::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		d2d::PrimitiveDraw::drawDashLine(anchorA, anchorB, d2d::Colorf(1, 0, 0), 2);
		d2d::PrimitiveDraw::drawDashLine(anchorA, bodyA->sprite->getPosition(), d2d::Colorf(0.4f, 0.8f, 0.4f), 2);
		d2d::PrimitiveDraw::drawDashLine(anchorB, bodyB->sprite->getPosition(), d2d::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag();
	}

	drawAnchor(anchorA, type);
	drawAnchor(anchorB, type);
}

d2d::Vector PrismaticJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

d2d::Vector PrismaticJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void PrismaticJoint::setLocalAnchorA(const d2d::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void PrismaticJoint::setLocalAnchorB(const d2d::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void PrismaticJoint::drawAnchor(const d2d::Vector& pos, DrawType type) const
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
