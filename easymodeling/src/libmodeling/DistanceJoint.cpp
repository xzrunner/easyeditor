
#include "DistanceJoint.h"
#include "Body.h"

using namespace libmodeling;

DistanceJoint::DistanceJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_distanceJoint)
	, frequencyHz(0.0f)
	, dampingRatio(0.0f)
{
}

bool DistanceJoint::isContain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool DistanceJoint::isIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(getWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(getWorldAnchorB(), rect);
}

void DistanceJoint::draw(DrawType type) const
{
	const ee::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::PrimitiveDraw::DrawDashLine(anchorA, bodyA->sprite->GetPosition(), ee::Colorf(0.4f, 0.8f, 0.4f), 2);
		ee::PrimitiveDraw::DrawDashLine(anchorB, bodyB->sprite->GetPosition(), ee::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag();
	}

	drawAnchor(anchorA, type);
	drawAnchor(anchorB, type);
	drawConnection(anchorA, anchorB, type);
}


ee::Vector DistanceJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

ee::Vector DistanceJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void DistanceJoint::setLocalAnchorA(const ee::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void DistanceJoint::setLocalAnchorB(const ee::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void DistanceJoint::drawAnchor(const ee::Vector& pos, DrawType type) const
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

void DistanceJoint::drawConnection(const ee::Vector& worldAnchorA, 
								   const ee::Vector& worldAnchorB, DrawType type) const
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

	ee::PrimitiveDraw::DrawLine(worldAnchorA, worldAnchorB, color, 1);
}
