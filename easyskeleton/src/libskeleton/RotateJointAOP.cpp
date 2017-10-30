#include "RotateJointAOP.h"
#include "Joint.h"

namespace libskeleton
{

RotateJointAOP::RotateJointAOP(const std::shared_ptr<Joint>& joint, float rot)
	: m_joint(joint)
	, m_rot(rot)
{
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