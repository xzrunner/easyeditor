
#include "WeldJoint.h"
#include "Body.h"

using namespace libmodeling;

WeldJoint::WeldJoint(Body* b0, Body* b1)
	: Joint(b0, b1, e_weldJoint)
	, referenceAngle(0.0f)
	, frequencyHz(0.0f)
	, dampingRatio(0.0f)
{
	ee::Vector center = (b0->sprite->GetPosition() + b1->sprite->GetPosition()) * 0.5f;
	setLocalAnchorA(center);
	setLocalAnchorB(center);
}

bool WeldJoint::isContain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT;
}

bool WeldJoint::isIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(getWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(getWorldAnchorB(), rect);
}

void WeldJoint::draw(DrawType type) const
{
	const ee::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::PrimitiveDraw::DrawDashLine(anchorA, anchorB, ee::Colorf(1, 0, 0), 2);
		ee::PrimitiveDraw::DrawDashLine(anchorA, bodyA->sprite->GetPosition(), ee::Colorf(0.4f, 0.8f, 0.4f), 2);
		ee::PrimitiveDraw::DrawDashLine(anchorB, bodyB->sprite->GetPosition(), ee::Colorf(0.4f, 0.4f, 0.8f), 2);

		drawBodyFlag();
	}

	drawAnchor(anchorA, type);
	drawAnchor(anchorB, type);
}

ee::Vector WeldJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

ee::Vector WeldJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void WeldJoint::setLocalAnchorA(const ee::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void WeldJoint::setLocalAnchorB(const ee::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void WeldJoint::drawAnchor(const ee::Vector& pos, DrawType type) const
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
