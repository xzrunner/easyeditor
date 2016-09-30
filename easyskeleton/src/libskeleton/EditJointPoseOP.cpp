#include "EditJointPoseOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>

#include <SM_Calc.h>
#include <sprite2/Joint.h>
#include <sprite2/Skeleton.h>
#include <sprite2/S2_RVG.h>

namespace libskeleton
{

EditJointPoseOP::EditJointPoseOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), false)
	, m_selected(NULL)
{
	const Sprite* spr = stage->GetSprite();
	const Symbol* sym = dynamic_cast<const Symbol*>(spr->GetSymbol());
	m_skeleton = sym->GetSkeleton();
	if (m_skeleton) {
		m_skeleton->AddReference();
	}

	m_last_left_pos.MakeInvalid();
	m_last_right_pos.MakeInvalid();
}

EditJointPoseOP::~EditJointPoseOP()
{
	if (m_selected) {
		m_selected->RemoveReference();
	}
	if (m_skeleton) {
		m_skeleton->RemoveReference();
	}
}

bool EditJointPoseOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Select(pos);
	m_last_left_pos = pos;

	return false;
}

bool EditJointPoseOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	m_last_left_pos.MakeInvalid();

	return false;
}

bool EditJointPoseOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Select(pos);
	m_last_right_pos = pos;

	return false;
}

bool EditJointPoseOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightUp(x, y)) {
		return true;
	}

	m_last_right_pos.MakeInvalid();

	return false;
}

bool EditJointPoseOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y)) {
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Select(pos);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

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
	// translate
	if (m_last_left_pos.IsValid()) 
	{
		m_selected->Translate(pos - m_last_left_pos);
		m_last_left_pos = pos;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	} 
	// rotate
	else if (m_last_right_pos.IsValid()) 
	{
		const s2::Sprite* spr = m_selected->GetSkinSpr();
		sm::vec2 center = spr->GetPosition() + spr->GetOffset();
		float rot = sm::get_angle_in_direction(center, m_last_right_pos, pos);
		m_selected->Rotate(rot);
		m_last_right_pos = pos;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditJointPoseOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) {
		return true;
	}

	if (m_selected) {
		s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
		s2::RVG::Circle(m_selected->GetWorldPos(), 8, true);
	}

	return false;
}

bool EditJointPoseOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) {
		return true;
	}

	return false;
}

void EditJointPoseOP::Select(const sm::vec2& pos)
{
	const s2::Joint* old_selected = m_selected;
	m_selected = const_cast<s2::Joint*>(m_skeleton->QueryByPos(pos));
	if (old_selected != m_selected) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

}