
#include "FrictionJoint.h"
#include "Body.h"

using namespace libmodeling;

FrictionJoint::FrictionJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_frictionJoint)
	, maxForce(0.0f)
	, maxTorque(0.0f)
{
	d2d::Vector center = (b0->sprite->GetPosition() + b1->sprite->GetPosition()) * 0.5f;
	setLocalAnchorA(center);
	setLocalAnchorB(center);
}

bool FrictionJoint::isContain(const d2d::Vector& pos) const
{
	return d2d::Math2D::GetDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| d2d::Math2D::GetDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool FrictionJoint::isIntersect(const d2d::Rect& rect) const
{
	return d2d::Math2D::IsPointInRect(getWorldAnchorA(), rect) 
		|| d2d::Math2D::IsPointInRect(getWorldAnchorB(), rect);
}

void FrictionJoint::draw(DrawType type) const
{
	const d2d::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		d2d::PrimitiveDraw::DrawDashLine(anchorA, anchorB, d2d::Colorf(1, 0, 0), 2);
		d2d::PrimitiveDraw::DrawDashLine(anchorA, bodyA->sprite->GetPosition(), d2d::Colorf(0.4f, 0.8f, 0.4f), 2);
		d2d::PrimitiveDraw::DrawDashLine(anchorB, bodyB->sprite->GetPosition(), d2d::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag();
	}

	drawAnchor(anchorA, type);
	drawAnchor(anchorB, type);
}

d2d::Vector FrictionJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

d2d::Vector FrictionJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void FrictionJoint::setLocalAnchorA(const d2d::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void FrictionJoint::setLocalAnchorB(const d2d::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void FrictionJoint::drawAnchor(const d2d::Vector& pos, DrawType type) const
{
	d2d::Colorf color;
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

	d2d::PrimitiveDraw::DrawCircle(pos, JOINT_RADIUS_IN, true, 2, color);
	d2d::PrimitiveDraw::DrawCircle(pos, JOINT_RADIUS_OUT, false, 2, color);
}
