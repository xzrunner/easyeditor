
#include "DistanceJoint.h"
#include "Body.h"

using namespace libmodeling;

DistanceJoint::DistanceJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_distanceJoint)
	, frequencyHz(0.0f)
	, dampingRatio(0.0f)
{
}

bool DistanceJoint::isContain(const d2d::Vector& pos) const
{
	return d2d::Math2D::GetDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| d2d::Math2D::GetDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool DistanceJoint::isIntersect(const d2d::Rect& rect) const
{
	return d2d::Math2D::IsPointInRect(getWorldAnchorA(), rect) 
		|| d2d::Math2D::IsPointInRect(getWorldAnchorB(), rect);
}

void DistanceJoint::draw(DrawType type) const
{
	const d2d::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		d2d::PrimitiveDraw::DrawDashLine(anchorA, bodyA->sprite->GetPosition(), d2d::Colorf(0.4f, 0.8f, 0.4f), 2);
		d2d::PrimitiveDraw::DrawDashLine(anchorB, bodyB->sprite->GetPosition(), d2d::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag();
	}

	drawAnchor(anchorA, type);
	drawAnchor(anchorB, type);
	drawConnection(anchorA, anchorB, type);
}


d2d::Vector DistanceJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

d2d::Vector DistanceJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void DistanceJoint::setLocalAnchorA(const d2d::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void DistanceJoint::setLocalAnchorB(const d2d::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void DistanceJoint::drawAnchor(const d2d::Vector& pos, DrawType type) const
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

void DistanceJoint::drawConnection(const d2d::Vector& worldAnchorA, 
								   const d2d::Vector& worldAnchorB, DrawType type) const
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

	d2d::PrimitiveDraw::DrawLine(worldAnchorA, worldAnchorB, color, 1);
}
