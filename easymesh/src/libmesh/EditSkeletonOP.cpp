#include "EditSkeletonOP.h"
#include "StagePanel.h"
#include "Mesh.h"
#include "Skeleton.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>

#include <SM_Calc.h>
#include <sprite2/MeshJoint.h>
#include <sprite2/DrawMesh.h>
#include <sprite2/RenderParams.h>
#include <sprite2/MeshSkeleton.h>
#include <sprite2/S2_RVG.h>

#include <queue>

namespace emesh
{

EditSkeletonOP::EditSkeletonOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_selected(NULL)
{
}

EditSkeletonOP::~EditSkeletonOP()
{
	if (m_selected) {
		m_selected->RemoveReference();
	}
}

bool EditSkeletonOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y))	{
		return true;
	}

	return false;
}

bool EditSkeletonOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightUp(x, y))	{
		return true;
	}

	return false;
}

bool EditSkeletonOP::OnMouseMove(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseMove(x, y))	{
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Select(pos);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool EditSkeletonOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y))	{
		return true;
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

		s2::MeshSkeleton& skeleton = static_cast<Skeleton*>(mesh)->GetSkeleton();
		skeleton.Draw(s2::RenderParams());
		
		if (m_selected && m_selected->GetParent()) 
		{
			const sm::vec2& cpos = m_selected->GetWorldPose().trans,
				ppos = m_selected->GetParent()->GetWorldPose().trans;
			s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
			s2::RVG::Circle(cpos, s2::MeshJoint::RADIUS, true);
			s2::RVG::Line(ppos, cpos);
			s2::RVG::Arrow(ppos, cpos, s2::MeshJoint::RADIUS * 2);
		}
	}

	return false;
}

bool EditSkeletonOP::Clear()
{
	if (ee::ZoomViewOP::Clear())	{
		return true;
	}

	if (m_selected) {
		m_selected->RemoveReference();
		m_selected = NULL;
	}

	return false;
}

void EditSkeletonOP::Select(const sm::vec2& pos)
{
	const s2::MeshJoint* root = NULL;
	if (Mesh* mesh = static_cast<StagePanel*>(m_wnd)->GetMesh()) {
		s2::MeshSkeleton& skeleton = static_cast<Skeleton*>(mesh)->GetSkeleton();
		root = skeleton.GetRoot();
	}
	if (!root) {
		return;
	}

	float nearest = FLT_MAX;
	std::queue<const s2::MeshJoint*> buf;
	buf.push(root);
	while (!buf.empty())
	{
		const s2::MeshJoint* node = buf.front(); buf.pop();
		const std::vector<s2::MeshJoint*>& children = node->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			s2::MeshJoint* cnode = children[i];
			float dis = sm::dis_pos_to_seg(pos, node->GetWorldPose().trans, cnode->GetWorldPose().trans);
			if (dis < nearest) {
				nearest = dis;
				cu::RefCountObjAssign(m_selected, cnode);
			}
			buf.push(cnode);
		}
	}
}

}