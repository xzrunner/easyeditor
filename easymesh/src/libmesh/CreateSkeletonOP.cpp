#include "CreateSkeletonOP.h"
#include "StagePanel.h"
#include "Skeleton.h"

#include <ee/SpriteRenderer.h>
#include <ee/panel_msg.h>

#include <sprite2/DrawMesh.h>
#include <sprite2/MeshJoint.h>
#include <sprite2/S2_RVG.h>

namespace emesh
{

CreateSkeletonOP::CreateSkeletonOP(StagePanel* stage)
	: eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>(stage, stage->GetStageImpl(), stage, NULL, new ee::OneFloatValueStatic(5), NULL)
	, m_stage(stage)
	, m_selected(NULL)
{
	SetLoop(true);
}

CreateSkeletonOP::~CreateSkeletonOP()
{
	if (m_selected) {
		m_selected->RemoveReference();
	}
}

bool CreateSkeletonOP::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Skeleton* mesh = static_cast<Skeleton*>(m_stage->GetMesh());
	if (!mesh) {
		return false;
	}

	if (const s2::PolylineShape* bound = mesh->GetShape())
	{
		s2::MeshSkeleton& skeleton = mesh->GetSkeleton();
		if (!skeleton.GetRoot()) {
			assert(!m_selected);
			m_selected = new s2::MeshJoint(NULL, pos);
			skeleton.SetRoot(m_selected);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			return false;
		}
 		s2::MeshJoint* selected = skeleton.QueryByPos(pos);
		if (selected) {
			cu::RefCountObjAssign(m_selected, selected);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			return false;
		} else if (m_selected) {
			s2::MeshJoint* joint = new s2::MeshJoint(m_selected, pos);
			cu::RefCountObjAssign(m_selected, joint);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			return false;
		}
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftDown(x, y)) {
		return true;
	}

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateSkeletonOP::OnMouseLeftUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateSkeletonOP::OnMouseRightDown(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightDown(x, y)) {
		return true;
	}

	Skeleton* mesh = static_cast<Skeleton*>(m_stage->GetMesh());
	if (mesh && mesh->GetShape())
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		s2::MeshSkeleton& skeleton = mesh->GetSkeleton();
		s2::MeshJoint* selected = skeleton.QueryByPos(pos);
		if (selected && selected != skeleton.GetRoot()) {
			if (m_selected) {
				m_selected->RemoveReference();
				m_selected = NULL;
			}
			selected->Deconnect();
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateSkeletonOP::OnMouseRightUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateSkeletonOP::OnMouseDrag(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_selected) {
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		m_selected->SetWorldPos(pos);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool CreateSkeletonOP::OnDraw() const
{
	if (Mesh* mesh = m_stage->GetMesh()) 
	{
		ee::SpriteRenderer::Instance()->Draw(mesh->GetBaseSymbol());
		s2::DrawMesh::DrawInfoUV(mesh);

		s2::MeshSkeleton& skeleton = static_cast<Skeleton*>(mesh)->GetSkeleton();
		skeleton.Draw(s2::RenderParams());
		if (m_selected) {
			s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
			s2::RVG::Circle(m_selected->GetWorldPose().trans, s2::MeshJoint::RADIUS, true);
		}
	}

	eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnDraw();

	return false;
}

bool CreateSkeletonOP::Clear()
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::Clear()) {
		return true;
	}

	if (m_selected) {
		m_selected->RemoveReference();
		m_selected = NULL;
	}

	return false;
}

}