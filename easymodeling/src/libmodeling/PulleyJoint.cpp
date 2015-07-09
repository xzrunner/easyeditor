
#include "PulleyJoint.h"
#include "Body.h"

using namespace libmodeling;

PulleyJoint::PulleyJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_pulleyJoint)
	, ratio(1.0f)
{
	groundAnchorA = b0->sprite->GetPosition();
	groundAnchorB = b1->sprite->GetPosition();
	groundAnchorA.y += 10;
	groundAnchorB.y += 10;

	d2d::Vector center = (b0->sprite->GetPosition() + b1->sprite->GetPosition()) * 0.5f;
	setLocalAnchorA(center);
	setLocalAnchorB(center);
}

bool PulleyJoint::isContain(const d2d::Vector& pos) const
{
	return d2d::Math::getDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| d2d::Math::getDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT
		|| d2d::Math::getDistance(groundAnchorA, pos) < JOINT_RADIUS_OUT
		|| d2d::Math::getDistance(groundAnchorB, pos) < JOINT_RADIUS_OUT;
}

bool PulleyJoint::isIntersect(const d2d::Rect& rect) const
{
	return d2d::Math::isPointInRect(getWorldAnchorA(), rect) 
		|| d2d::Math::isPointInRect(getWorldAnchorB(), rect)
		|| d2d::Math::isPointInRect(groundAnchorA, rect) 
		|| d2d::Math::isPointInRect(groundAnchorB, rect);
}

void PulleyJoint::draw(DrawType type) const
{
	const d2d::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		d2d::PrimitiveDraw::drawDashLine(anchorA, anchorB, d2d::Colorf(1, 0, 0), 2);
		d2d::PrimitiveDraw::drawDashLine(anchorA, bodyA->sprite->GetPosition(), d2d::Colorf(0.4f, 0.8f, 0.4f), 2);
		d2d::PrimitiveDraw::drawDashLine(anchorB, bodyB->sprite->GetPosition(), d2d::Colorf(0.4f, 0.4f, 0.8f), 2);

		d2d::PrimitiveDraw::drawLine(anchorA, groundAnchorA, d2d::Colorf(0.8f, 0.8f, 0.4f));
		d2d::PrimitiveDraw::drawLine(anchorB, groundAnchorB, d2d::Colorf(0.8f, 0.8f, 0.4f));
		d2d::PrimitiveDraw::drawLine(groundAnchorA, groundAnchorB, d2d::Colorf(0.8f, 0.8f, 0.4f));

		drawBodyFlag();
	}

	drawAnchor(anchorA, type);
	drawAnchor(anchorB, type);
	drawAnchor(groundAnchorA, type);
	drawAnchor(groundAnchorB, type);
}


d2d::Vector PulleyJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

d2d::Vector PulleyJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void PulleyJoint::setLocalAnchorA(const d2d::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void PulleyJoint::setLocalAnchorB(const d2d::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void PulleyJoint::drawAnchor(const d2d::Vector& pos, DrawType type) const
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