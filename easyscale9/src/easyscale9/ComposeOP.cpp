#include "ComposeOP.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "ComposeImpl.h"
#include "ComposeGrids.h"

namespace escale9
{

ComposeOP::ComposeOP(ToolbarPanel* toolbar, StagePanel* stage, 
					 d2d::PropertySettingPanel* property,
					 d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(stage, stage->GetStageImpl(), stage, property, view_panel_mgr, NULL, 
	d2d::ArrangeSpriteConfig(false, false, false, false), new ComposeImpl(stage, property))
	, m_toolbar(toolbar)
{
}

bool ComposeOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::OnMouseLeftDown(x, y)) {
		return true;
	}

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	int col, row;
	ComposeGrids::Query(pos, &col, &row);
	if (col == -1 || row == -1) {
		return false;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	d2d::ISprite* spr = stage->getSprite(row, col);
	stage->GetSpriteSelection()->Clear();
	stage->GetSpriteSelection()->Add(spr);

	return false;
}

bool ComposeOP::OnActive()
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::OnActive()) return true;

	m_toolbar->setComposeOP(true);

	return false;
}

} // escale9