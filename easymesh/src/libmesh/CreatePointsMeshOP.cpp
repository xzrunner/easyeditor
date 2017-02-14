#include "CreatePointsMeshOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Mesh.h"
#include "PointsMesh.h"

#include <ee/Image.h>
#include <ee/panel_msg.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/DrawMesh.h>

namespace emesh
{

CreatePointsMeshOP::CreatePointsMeshOP(StagePanel* stage)
	: eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>(stage, stage->GetStageImpl(), stage, NULL, new ee::OneFloatValueStatic(5), NULL)
	, m_stage(stage)
	, m_selected_inner(-1)
{
	SetLoop(true);
}

bool CreatePointsMeshOP::OnKeyDown(int keyCode)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnKeyDown(keyCode))
		return true;

// 	switch (keyCode)
// 	{
// 	case 'a': case 'A':
// 	case 'd': case 'D':
// 	case 's': case 'S':
// 	case 'w': case 'W':
// 		if (IsDirty()) {
// 			ResetDirty();
// 		}
// 		break;
// 	}

	return false;
}

bool CreatePointsMeshOP::OnMouseLeftDown(int x, int y)
{
	PointsMesh* mesh = dynamic_cast<PointsMesh*>(m_stage->GetMesh());
	if (!mesh) {
		return false;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	int idx = mesh->QueryInnerPos(pos);
	if (idx != -1) {
		m_selected_inner = idx;
		return false;
	} else if (mesh->InsertInnerPos(pos)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return false;
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftDown(x, y)) {
		return true;
	}

	if (IsDirty()) {
		ResetDirty();
	}

	return false;
}

bool CreatePointsMeshOP::OnMouseLeftUp(int x, int y)
{
	if (m_selected_inner != -1) {
		m_selected_inner = -1;
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftUp(x, y))
		return true;

	if (IsDirty()) {
		ResetDirty();
	}

	return false;
}

bool CreatePointsMeshOP::OnMouseRightDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	PointsMesh* points_mesh = static_cast<PointsMesh*>(m_stage->GetMesh());
	if (points_mesh->RemoveInnerPos(pos)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return false;
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightDown(x, y)) {
		return true;
	}

	if (IsDirty()) {
		ResetDirty();
	}

	return false;
}

bool CreatePointsMeshOP::OnMouseRightUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightUp(x, y))
		return true;

	if (IsDirty()) {
		ResetDirty();
	}

	return false;
}

bool CreatePointsMeshOP::OnMouseDrag(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_selected_inner == -1) {
		return false;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	PointsMesh* points_mesh = static_cast<PointsMesh*>(m_stage->GetMesh());
	if (points_mesh->MoveInnerPos(m_selected_inner, pos)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	}

	return false;
}

bool CreatePointsMeshOP::OnDraw() const
{
	if (Mesh* mesh = m_stage->GetMesh()) {
		ee::SpriteRenderer::Instance()->Draw(mesh->GetBaseSymbol());
		s2::DrawMesh::DrawInfoUV(mesh);
	}

	eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnDraw();

	return false;
}

bool CreatePointsMeshOP::Clear()
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::Clear()) {
		return true;
	}
	m_selected_inner = -1;
	return false;
}

}