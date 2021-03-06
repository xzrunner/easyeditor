#include "CreateStripMeshOP.h"
#include "StagePanel.h"
#include "StripMesh.h"
#include "Symbol.h"
#include "Mesh.h"

#include <ee/panel_msg.h>
#include <ee/Image.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/DrawMesh.h>

namespace emesh
{

CreateStripOP::CreateStripOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true, false)
	, m_stage(stage)
	, m_selected(NULL)
{
	m_last_right.MakeInvalid();
}

bool CreateStripOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y))
		return true;

// 	if (StripMesh* strip = static_cast<StripMesh*>(m_stage->GetMesh()))
// 	{
// 		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
// 		m_selected = strip->FindNode(pos);
// 	}

	return false;
}

bool CreateStripOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y))
		return true;

	if (m_selected) {
		return false;
	}

// 	if (StripMesh* strip = static_cast<StripMesh*>(m_stage->GetMesh()))
// 	{
// 		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
// 		strip->InsertNode(pos);
// 		ee::SetCanvasDirtySJ::Instance()->SetDirty();
// 	}

	return false;
}

bool CreateStripOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y))
		return true;

// 	if (StripMesh* strip = static_cast<StripMesh*>(m_stage->GetMesh()))
// 	{
// 		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
// 		strip->RemoveNode(pos);
// 		ee::SetCanvasDirtySJ::Instance()->SetDirty();
// 		m_last_right = pos;
// 	}

	return false;
}

bool CreateStripOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightUp(x, y))
		return true;

	m_last_right.MakeInvalid();

	return false;
}

bool CreateStripOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y))
		return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	// move background
	if (m_last_right.IsValid())
	{
		sm::vec2 offset = pos - m_last_right;
		StagePanel* stage = static_cast<StagePanel*>(m_stage);
		stage->TranslateBackground(offset);
		m_last_right = pos;
	}

	if (!m_selected) {
		return false;
	}

// 	if (StripMesh* strip = static_cast<StripMesh*>(m_stage->GetMesh()))
// 	{
// 		strip->MoveNode(m_selected, pos);
// 		ee::SetCanvasDirtySJ::Instance()->SetDirty();
// 	}

	return false;
}

bool CreateStripOP::OnDraw() const
{
	auto mesh = m_stage->GetMesh();
	if (mesh) {
		ee::SpriteRenderer::Instance()->Draw(mesh->GetBaseSymbol().get());
		s2::DrawMesh::DrawInfoUV(nullptr, *mesh);
	}

	ee::ZoomViewOP::OnDraw();

	return false;
}

bool CreateStripOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) {
		return true;
	}

	m_selected = NULL;
	m_last_right.MakeInvalid();

	return false;
}

}