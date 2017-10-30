#include "TranslateJointAOP.h"
#include "Joint.h"

namespace libskeleton
{

TranslateJointAOP::TranslateJointAOP(const std::shared_ptr<Joint>& joint, const sm::vec2& trans)
	: m_joint(joint)
	, m_trans(trans)
{
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