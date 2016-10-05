#include "ScaleJointAOP.h"
#include "Joint.h"

namespace libskeleton
{

ScaleJointAOP::ScaleJointAOP(Joint* joint, const sm::vec2& scale)
	: m_joint(joint)
	, m_scale(scale)
{
	if (m_joint) {
		m_joint->AddReference();
	}
}

ScaleJointAOP::~ScaleJointAOP()
{
	if (m_joint) {
		m_joint->RemoveReference();
	}
}

void ScaleJointAOP::Undo()
{
	if (m_scale.x != 0 && m_scale.y != 0) {
		m_joint->Scale(sm::vec2(1 / m_scale.x, 1 / m_scale.y));
	}
}

void ScaleJointAOP::Redo()
{
	m_joint->Scale(m_scale);
}

}