#include "EditJointPoseOP.h"
#include "TranslateJointState.h"
#include "RotateJointState.h"
#include "ScaleJointState.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"
#include "Joint.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>

#include <sprite2/Skeleton.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/RenderParams.h>

namespace libskeleton
{

EditJointPoseOP::EditJointPoseOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), false)
	, m_sk(stage->GetSkeleton())
	, m_selected(NULL)
	, m_op_state(NULL)
{
	if (m_sk) {
		m_sk->AddReference();
	}
}

EditJointPoseOP::~EditJointPoseOP()
{
	if (m_sk) {
		m_sk->RemoveReference();
	}
	if (m_selected) {
		m_selected->RemoveReference();
	}
	if (m_op_state) {
		delete m_op_state;
	}
}

bool EditJointPoseOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Select(pos);

	if (m_selected) {
		if (m_stage->GetKeyState(WXK_CONTROL)) {
			ChangeOPState(new ScaleJointState(m_selected, pos));
		} else {
			ChangeOPState(new TranslateJointState(m_selected, pos));
		}
		m_op_state->OnMousePress(pos);
	}

	return false;
}

bool EditJointPoseOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state) 
	{
		m_op_state->OnMouseRelease(pos);
		ChangeOPState(NULL);
	}	

	return false;
}

bool EditJointPoseOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Select(pos);

	if (m_selected) {
		ChangeOPState(new RotateJointState(m_selected, pos));
		m_op_state->OnMousePress(pos);
	}

	return false;
}

bool EditJointPoseOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightUp(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state) 
	{
		m_op_state->OnMouseRelease(pos);
		ChangeOPState(NULL);
	}

	return false;
}

bool EditJointPoseOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (Select(pos)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	if (m_op_state) {
		m_op_state->OnMouseMove(pos);
	}

	return false;
}

bool EditJointPoseOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (!m_selected) {
		return false;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state && m_op_state->OnMouseDrag(pos)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditJointPoseOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) {
		return true;
	}
	if (!m_sk) {
		return false;
	}

	s2::RenderParams params;
	const std::vector<s2::Joint*>& joints = m_sk->GetAllJoints();
	for (int i = 0, n = joints.size(); i < n; ++i) {
		s2::Joint* joint = joints[i];
		dynamic_cast<Joint*>(joint)->DrawSkeleton(params, joint == m_selected);
	}

	return false;
}

bool EditJointPoseOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) {
		return true;
	}

	if (m_selected) {
		m_selected->RemoveReference();
		m_selected = NULL;
	}
	if (m_op_state) {
		delete m_op_state;
		m_op_state = NULL;
	}

	return false;
}

bool EditJointPoseOP::Select(const sm::vec2& pos)
{
	if (!m_sk) {
		return false;
	}

	s2::Joint* old_selected = m_selected;
	const s2::Joint* joint = m_sk->QueryByPos(pos);
	m_selected = dynamic_cast<Joint*>(const_cast<s2::Joint*>(joint));
	return old_selected != m_selected;
}

void EditJointPoseOP::ChangeOPState(ee::ArrangeSpriteState* state)
{
	if (m_op_state) {
		delete m_op_state;
	}
	m_op_state = state;
}

}