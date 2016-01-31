
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

	ee::Vector center = (b0->sprite->GetPosition() + b1->sprite->GetPosition()) * 0.5f;
	setLocalAnchorA(center);
	setLocalAnchorB(center);
}

bool PulleyJoint::isContain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(getWorldAnchorA(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(getWorldAnchorB(), pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(groundAnchorA, pos) < JOINT_RADIUS_OUT
		|| ee::Math2D::GetDistance(groundAnchorB, pos) < JOINT_RADIUS_OUT;
}

bool PulleyJoint::isIntersect(const ee::Rect& rect) const
{
	return ee::Math2D::IsPointInRect(getWorldAnchorA(), rect) 
		|| ee::Math2D::IsPointInRect(getWorldAnchorB(), rect)
		|| ee::Math2D::IsPointInRect(groundAnchorA, rect) 
		|| ee::Math2D::IsPointInRect(groundAnchorB, rect);
}

void PulleyJoint::draw(DrawType type) const
{
	const ee::Vector anchorA = getWorldAnchorA(),
		anchorB = getWorldAnchorB();

	if (type == e_selected || type == e_mouseOn)
	{
		ee::PrimitiveDraw::DrawDashLine(anchorA, anchorB, ee::Colorf(1, 0, 0), 2);
		ee::PrimitiveDraw::DrawDashLine(anchorA, bodyA->sprite->GetPosition(), ee::Colorf(0.4f, 0.8f, 0.4f), 2);
		ee::PrimitiveDraw::DrawDashLine(anchorB, bodyB->sprite->GetPosition(), ee::Colorf(0.4f, 0.4f, 0.8f), 2);

		ee::PrimitiveDraw::DrawLine(anchorA, groundAnchorA, ee::Colorf(0.8f, 0.8f, 0.4f));
		ee::PrimitiveDraw::DrawLine(anchorB, groundAnchorB, ee::Colorf(0.8f, 0.8f, 0.4f));
		ee::PrimitiveDraw::DrawLine(groundAnchorA, groundAnchorB, ee::Colorf(0.8f, 0.8f, 0.4f));

		drawBodyFlag();
	}

	drawAnchor(anchorA, type);
	drawAnchor(anchorB, type);
	drawAnchor(groundAnchorA, type);
	drawAnchor(groundAnchorB, type);
}


ee::Vector PulleyJoint::getWorldAnchorA() const
{
	return transLocalToWorld(localAnchorA, bodyA->sprite);
}

ee::Vector PulleyJoint::getWorldAnchorB() const
{
	return transLocalToWorld(localAnchorB, bodyB->sprite);
}

void PulleyJoint::setLocalAnchorA(const ee::Vector& world)
{
	localAnchorA = transWorldToLocal(world, bodyA->sprite);
}

void PulleyJoint::setLocalAnchorB(const ee::Vector& world)
{
	localAnchorB = transWorldToLocal(world, bodyB->sprite);
}

void PulleyJoint::drawAnchor(const ee::Vector& pos, DrawType type) const
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