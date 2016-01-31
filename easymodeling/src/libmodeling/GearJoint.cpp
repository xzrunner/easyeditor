
#include "GearJoint.h"

using namespace libmodeling;

GearJoint::GearJoint(Body* b0, Body* b1, Joint* j1, Joint* j2)
	: Joint(b0, b1, e_gearJoint)
	, joint1(j1)
	, joint2(j2)
	, ratio(1.0f)
{
}

bool GearJoint::isContain(const ee::Vector& pos) const
{
	return joint1->isContain(pos) || joint2->isContain(pos);
}

bool GearJoint::isIntersect(const ee::Rect& rect) const
{
	return joint1->isIntersect(rect) || joint2->isIntersect(rect);
}

void GearJoint::draw(DrawType type) const
{
	joint1->draw(type);
	joint2->draw(type);
}