#include "ComposeOP.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

namespace escale9
{

ComposeOP::ComposeOP(StagePanel* stage, ToolbarPanel* toolbar)
	: d2d::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_toolbar(toolbar)
{
}

bool ComposeOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	SelectByPos(x, y);

	return false;
}

bool ComposeOP::OnMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseRightDown(x, y)) {
		return true;
	}

	d2d::ISprite* selected = SelectByPos(x, y);
	if (selected) {
		selected->SetTransform(selected->GetPosition(), selected->GetAngle() + d2d::PI*0.5f);
		m_stage->SetCanvasDirty();
	}

	return false;
}

bool ComposeOP::OnActive()
{
	if (d2d::ZoomViewOP::OnActive()) {
		return true;
	}

	m_toolbar->setComposeOP(true);

	return false;
}

bool ComposeOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->GetSpriteSelection()->Traverse(d2d::DrawSelectedSpriteVisitor(d2d::Colorf(1, 0, 0)));

	return false;
}

d2d::ISprite* ComposeOP::SelectByPos(int x, int y)
{
	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	int col, row;
	ComposeGrids::Query(pos, &col, &row);
	if (col == -1 || row == -1) {
		return NULL;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	d2d::ISprite* selected = stage->getSprite(row, col);
	if (selected) {
		d2d::SpriteSelection* selection = stage->GetSpriteSelection();
		selection->Clear();
		selection->Add(selected);

		d2d::SelectSpriteSJ::Instance()->Select(selected, true);
	}

	return selected;
}

} // escale9