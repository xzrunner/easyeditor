#include "GearJoint.h"

namespace emodeling
{

GearJoint::GearJoint(Body* b0, Body* b1, Joint* j1, Joint* j2)
	: Joint(b0, b1, e_gearJoint)
	, m_joint1(j1)
	, m_joint2(j2)
	, m_ratio(1.0f)
{
}

bool GearJoint::IsContain(const ee::Vector& pos) const
{
	return m_joint1->IsContain(pos) || m_joint2->IsContain(pos);
}

bool GearJoint::IsIntersect(const ee::Rect& rect) const
{
	return m_joint1->IsIntersect(rect) || m_joint2->IsIntersect(rect);
}

void GearJoint::Draw(DrawType type) const
{
	m_joint1->Draw(type);
	m_joint2->Draw(type);
}

}