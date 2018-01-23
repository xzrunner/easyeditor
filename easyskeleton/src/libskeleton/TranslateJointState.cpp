#include "TranslateJointState.h"
#include "TranslateJointAOP.h"
#include "Joint.h"

#include <ee/panel_msg.h>

namespace libskeleton
{

TranslateJointState::TranslateJointState(const std::shared_ptr<Joint>& joint, const sm::vec2& first_pos)
	: m_joint(joint)
{
	m_first_pos = m_last_pos = first_pos;
}

void TranslateJointState::OnMouseRelease(const sm::vec2& pos)
{
	if (m_joint && pos != m_first_pos) {
		auto aop = std::make_shared<TranslateJointAOP>(m_joint, pos - m_first_pos);
		ee::EditAddRecordSJ::Instance()->Add(aop);
	}
}

bool TranslateJointState::OnMouseDrag(const sm::vec2& pos)
{
	if (!m_joint) return false;

	Translate(pos - m_last_pos);
	m_last_pos = pos;

	return true;
}

void TranslateJointState::Translate(const sm::vec2& trans)
{
	m_joint->Translate(trans);
}

}