#include "RotateJointState.h"
#include "RotateJointAOP.h"
#include "Joint.h"

#include <ee/panel_msg.h>

#include <SM_Calc.h>
#include <sprite2/Sprite.h>

namespace libskeleton
{

RotateJointState::RotateJointState(const std::shared_ptr<Joint>& joint, const sm::vec2& first_pos)
	: m_joint(joint)
	, m_angle(0)
{
	m_first_pos = m_last_pos = first_pos;
}

void RotateJointState::OnMouseRelease(const sm::vec2& pos)
{
	if (m_joint && pos != m_first_pos) {
		auto aop = std::make_shared<RotateJointAOP>(m_joint, m_angle);
		ee::EditAddRecordSJ::Instance()->Add(aop);
	}
}

bool RotateJointState::OnMouseDrag(const sm::vec2& pos)
{
	if (!m_joint) return false;

	Rotate(pos);
	m_last_pos = pos;

	return true;
}

void RotateJointState::Rotate(const sm::vec2& dst)
{
	float rot = sm::get_angle_in_direction(m_joint->GetWorldPose().trans, m_last_pos, dst);
	m_joint->Rotate(rot);
	m_angle += rot;
}

}