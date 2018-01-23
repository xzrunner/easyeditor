#include "ScaleJointState.h"
#include "ScaleJointAOP.h"
#include "Joint.h"

#include <ee/panel_msg.h>

namespace libskeleton
{

ScaleJointState::ScaleJointState(const std::shared_ptr<Joint>& joint, const sm::vec2& first_pos)
	: m_joint(joint)
	, m_scale(1, 1)
{
	m_first_pos = m_last_pos = first_pos;
}

void ScaleJointState::OnMouseRelease(const sm::vec2& pos)
{
	if (m_joint && pos != m_first_pos) {
 		auto aop = std::make_shared<ScaleJointAOP>(m_joint, m_scale);
 		ee::EditAddRecordSJ::Instance()->Add(aop);
	}
}

bool ScaleJointState::OnMouseDrag(const sm::vec2& pos)
{
	if (!m_joint) return false;

	Scale(pos);
	m_last_pos = pos;

	return true;
}

void ScaleJointState::Scale(const sm::vec2& dst)
{
	sm::vec2 center = m_joint->GetWorldPose().trans;
	if (center.x == dst.x || center.y == dst.y) {
		return;
	}
	sm::vec2 scale = (dst - center) / (m_last_pos - center);
	m_joint->Scale(scale);
	m_scale *= scale;
}

}