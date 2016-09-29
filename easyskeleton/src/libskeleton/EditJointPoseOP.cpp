#include "EditJointPoseOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>

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
	m_trans = spr->GetTransMatrix();
	m_inv_trans = spr->GetTransInvMatrix();
	const Symbol* sym = dynamic_cast<const Symbol*>(spr->GetSymbol());
	m_skeleton = sym->GetSkeleton();
	if (m_skeleton) {
		m_skeleton->AddReference();
	}
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

	const s2::Joint* old_selected = m_selected;
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	pos = m_inv_trans * pos;
	m_selected = const_cast<s2::Joint*>(m_skeleton->QueryByPos(pos));
	if (old_selected != m_selected) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditJointPoseOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	return false;
}

bool EditJointPoseOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) {
		return true;
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
		s2::RVG::Circle(m_trans * m_selected->GetWorldPos(), 8, true);
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

}