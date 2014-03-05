#include "ComposeOP.h"
#include "Context.h"
#include "StagePanel.h"

namespace escale9
{

ComposeOP::ComposeOP()
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(Context::Instance()->stage, Context::Instance()->stage, Context::Instance()->property)
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
	const int col = pos.x / Context::Instance()->EDGE,
		row = pos.y / Context::Instance()->EDGE;
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

	Context::Instance()->isComposeOP = true;

	return false;
}
} // escale9