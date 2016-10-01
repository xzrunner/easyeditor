#include "RotateJointAOP.h"
#include "Joint.h"

namespace libskeleton
{

RotateJointAOP::RotateJointAOP(Joint* joint, float rot)
	: m_joint(joint)
	, m_rot(rot)
{
	if (m_joint) {
		m_joint->AddReference();
	}
}

RotateJointAOP::~RotateJointAOP()
{
	if (m_joint) {
		m_joint->RemoveReference();
	}
}

void RotateJointAOP::Undo()
{
	m_joint->Rotate(-m_rot);	
}

void RotateJointAOP::Redo()
{
	m_joint->Rotate(m_rot);
}

}