#include "TranslateJointAOP.h"
#include "Joint.h"

namespace libskeleton
{

TranslateJointAOP::TranslateJointAOP(Joint* joint, const sm::vec2& trans)
	: m_joint(joint)
	, m_trans(trans)
{
	if (m_joint) {
		m_joint->AddReference();
	}
}

TranslateJointAOP::~TranslateJointAOP()
{
	if (m_joint) {
		m_joint->RemoveReference();
	}
}

void TranslateJointAOP::Undo()
{
	m_joint->Translate(-m_trans);
}

void TranslateJointAOP::Redo()
{
	m_joint->Translate(m_trans);
}

}