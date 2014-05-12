#include "ComposeOP.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "config.h"

namespace escale9
{

ComposeOP::ComposeOP(ToolbarPanel* toolbar, StagePanel* stage, 
					 d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(stage, stage, property)
	, m_toolbar(toolbar)
{
}

bool ComposeOP::onMouseRightDown(int x, int y)
{
// 	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
// 	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
// 
// 	if (selected)
// 	{
// 		selected->setTransform(selected->getPosition(), selected->getAngle() + d2d::PI*0.5f);
// 		m_editPanel->Refresh();
// 	}

	//////////////////////////////////////////////////////////////////////////

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	const int col = pos.x / EDGE,
		row = pos.y / EDGE;
	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	d2d::ISprite* selected = stage->getSprite(row, col);
 	if (selected)
 	{
 		selected->setTransform(selected->getPosition(), selected->getAngle() + d2d::PI*0.5f);
 		m_editPanel->Refresh();
 	}

	return false;
}

bool ComposeOP::onActive()
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onActive()) return true;

	m_toolbar->setComposeOP(true);

	return false;
}

} // escale9