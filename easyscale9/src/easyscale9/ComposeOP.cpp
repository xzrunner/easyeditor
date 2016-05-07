#include "ComposeOP.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/SpriteSelection.h>
#include <ee/DrawSelectedSpriteVisitor.h>
#include <ee/EditPanelImpl.h>
#include <ee/sprite_msg.h>

namespace escale9
{

ComposeOP::ComposeOP(StagePanel* stage, ToolbarPanel* toolbar)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_toolbar(toolbar)
{
}

bool ComposeOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	SelectByPos(x, y);

	return false;
}

bool ComposeOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) {
		return true;
	}

	ee::Sprite* selected = SelectByPos(x, y);
	if (selected) {
		selected->SetTransform(selected->GetPosition(), selected->GetAngle() + SM_PI*0.5f);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool ComposeOP::OnActive()
{
	if (ee::ZoomViewOP::OnActive()) {
		return true;
	}

	m_toolbar->setComposeOP(true);

	return false;
}

bool ComposeOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->GetSpriteSelection()->Traverse(ee::DrawSelectedSpriteVisitor(ee::Colorf(1, 0, 0)));

	return false;
}

ee::Sprite* ComposeOP::SelectByPos(int x, int y)
{
	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	int col, row;
	ComposeGrids::Query(pos, &col, &row);
	if (col == -1 || row == -1) {
		return NULL;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	ee::Sprite* selected = stage->getSprite(row, col);
	if (selected) {
		ee::SpriteSelection* selection = stage->GetSpriteSelection();
		selection->Clear();
		selection->Add(selected);

		ee::SelectSpriteSJ::Instance()->Select(selected, true);
	}

	return selected;
}

} // escale9