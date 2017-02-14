#include "EditSkeletonOP.h"
#include "StagePanel.h"
#include "Mesh.h"
#include "SkeletonMesh.h"
#include "RotateJointState.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>

#include <SM_Calc.h>
#include <sprite2/DrawMesh.h>
#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>

#include <queue>

namespace emesh
{

EditSkeletonOP::EditSkeletonOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_selected(NULL)
	, m_op_state(NULL)
{
}

EditSkeletonOP::~EditSkeletonOP()
{
// 	if (m_selected) {
// 		m_selected->RemoveReference();
// 	}
	if (m_op_state) {
		delete m_op_state;
	}
}

bool EditSkeletonOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y))	{
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Select(pos);

// 	if (m_selected) {
// 		ChangeOPState(new RotateJointState(m_selected, pos));
// 		m_op_state->OnMousePress(pos);
// 	}

	return false;
}

bool EditSkeletonOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightUp(x, y))	{
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

bool EditSkeletonOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y))	{
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

bool EditSkeletonOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y))	{
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

bool EditSkeletonOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw())	{
		return true;
	}

	if (Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh())
	{
		s2::DrawMesh::DrawTexture(mesh, s2::RenderParams());
		s2::DrawMesh::DrawInfoXY(mesh);

// 		s2::MeshSkeleton& skeleton = static_cast<SkeletonMesh*>(mesh)->GetSkeleton();
// 		skeleton.Draw(s2::RenderParams());
		
// 		if (m_selected) {
// 			s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
// 			s2::RVG::Circle(m_selected->GetWorldPose().trans, s2::MeshJoint::RADIUS, true);
// 		}
	}

	return false;
}

bool EditSkeletonOP::Clear()
{
	if (ee::ZoomViewOP::Clear())	{
		return true;
	}

// 	if (m_selected) {
// 		m_selected->RemoveReference();
// 		m_selected = NULL;
// 	}
	if (m_op_state) {
		delete m_op_state;
		m_op_state = NULL;
	}

	return false;
}

bool EditSkeletonOP::Select(const sm::vec2& pos)
{
// 	const s2::MeshJoint* root = NULL;
// 	if (Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh()) {
// 		s2::MeshSkeleton& skeleton = static_cast<SkeletonMesh*>(mesh)->GetSkeleton();
// 		root = skeleton.GetRoot();
// 	}
// 	if (!root) {
// 		return false;
// 	}
// 
// 	s2::MeshJoint* old_selected = m_selected;
// 
// 	float nearest = FLT_MAX;
// 	std::queue<const s2::MeshJoint*> buf;
// 	buf.push(root);
// 	while (!buf.empty())
// 	{
// 		const s2::MeshJoint* node = buf.front(); buf.pop();
// 		const std::vector<s2::MeshJoint*>& children = node->GetChildren();
// 		for (int i = 0, n = children.size(); i < n; ++i) {
// 			s2::MeshJoint* cnode = children[i];
// 			float dis = sm::dis_pos_to_seg(pos, node->GetWorldPose().trans, cnode->GetWorldPose().trans);
// 			if (dis < nearest) {
// 				nearest = dis;
// 				cu::RefCountObjAssign(m_selected, cnode);
// 			}
// 			buf.push(cnode);
// 		}
// 	}
// 
// 	return old_selected != m_selected;

	return false;
}

void EditSkeletonOP::ChangeOPState(ee::ArrangeSpriteState* state)
{
	if (m_op_state) {
		delete m_op_state;
	}
	m_op_state = state;
}

}