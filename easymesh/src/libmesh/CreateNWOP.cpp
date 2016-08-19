#include "CreateNWOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Mesh.h"
#include "Network.h"
#include "MeshRenderer.h"

#include <ee/Image.h>
#include <ee/panel_msg.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/NodeRenderer.h>

namespace emesh
{

CreateNWOP::CreateNWOP(StagePanel* stage)
	: eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>(stage, stage->GetStageImpl(), stage, NULL, new ee::OneFloatValueStatic(5), NULL)
	, m_stage(stage)
	, m_selected_inner(NULL)
{
	SetLoop(true);
}

bool CreateNWOP::OnKeyDown(int keyCode)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnKeyDown(keyCode))
		return true;

	switch (keyCode)
	{
	case 'a': case 'A':
	case 'd': case 'D':
	case 's': case 'S':
	case 'w': case 'W':
		if (IsDirty()) {
			m_stage->UpdateSymbol();
			ResetDirty();
		}
		break;
	}

	return false;
}

bool CreateNWOP::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Network* nw = static_cast<Network*>(m_stage->GetMesh());
	if (!nw) {
		return false;
	}
	sm::vec2* selected = nw->QueryInner(pos);
	if (selected) {
		m_selected_inner = selected;
		return false;
	} else if (nw->InsertInner(pos)) {
		RefreshAll();
		return false;
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

bool CreateNWOP::OnMouseLeftUp(int x, int y)
{
	if (m_selected_inner) {
		m_selected_inner = NULL;
		RefreshAll();
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNWOP::OnMouseRightDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Network* nw = static_cast<Network*>(m_stage->GetMesh());
	if (nw->RemoveInner(pos)) {
		RefreshAll();
		return false;
	}

	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightDown(x, y)) {
		return true;
	}

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNWOP::OnMouseRightUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNWOP::OnMouseDrag(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_selected_inner) {
		*m_selected_inner = m_stage->TransPosScrToProj(x, y);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool CreateNWOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw())
		return true;

	if (Mesh* mesh = m_stage->GetMesh()) {
		s2::NodeRenderer::Draw(mesh->GetBaseSymbol());
		MeshRenderer::DrawInfoUV(mesh);
	}

	eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnDraw();

	return false;
}

void CreateNWOP::RefreshAll()
{
	m_stage->UpdateSymbol();
	ResetDirty();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}