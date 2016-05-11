#include "CreateStripOP.h"
#include "StagePanel.h"
#include "Strip.h"
#include "Symbol.h"
#include "MeshRenderer.h"

#include <ee/panel_msg.h>
#include <ee/Image.h>
#include <ee/SpriteRenderer.h>

namespace emesh
{

CreateStripOP::CreateStripOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true, false)
	, m_stage(stage)
	, m_selected(NULL)
	, m_last_right_valid(false)
{
}

bool CreateStripOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y))
		return true;

	if (Strip* strip = static_cast<Strip*>(m_stage->GetMesh()))
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		m_selected = strip->FindNode(pos);
	}

	return false;
}

bool CreateStripOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y))
		return true;

	if (m_selected) {
		return false;
	}

	if (Strip* strip = static_cast<Strip*>(m_stage->GetMesh()))
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		strip->InsertNode(pos);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool CreateStripOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y))
		return true;

	if (Strip* strip = static_cast<Strip*>(m_stage->GetMesh()))
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		strip->RemoveNode(pos);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();

		m_last_right = pos;
		m_last_right_valid = true;
	}

	return false;
}

bool CreateStripOP::OnMouseRightUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightUp(x, y))
		return true;

	m_last_right_valid = false;

	return false;
}

bool CreateStripOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y))
		return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	// move background
	if (m_last_right_valid)
	{
		sm::vec2 offset = pos - m_last_right;
		StagePanel* stage = static_cast<StagePanel*>(m_stage);
		stage->TranslateBackground(offset);
		m_last_right = pos;
		m_last_right_valid = true;
	}

	if (!m_selected) {
		return false;
	}

	if (Strip* strip = static_cast<Strip*>(m_stage->GetMesh()))
	{
		strip->MoveNode(m_selected, pos);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool CreateStripOP::OnDraw() const
{
	if (Mesh* mesh = m_stage->GetMesh()) {
		ee::SpriteRenderer::Draw(mesh->GetBaseSymbol());
		MeshRenderer::DrawInfoUV(mesh);
	}

	ee::ZoomViewOP::OnDraw();

	return false;
}

}