#include "ComposeImpl.h"
#include "StagePanel.h"
#include "config.h"

namespace escale9
{

ComposeImpl::ComposeImpl(StagePanel* stage, d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteImpl(stage, stage, property, d2d::ArrangeSpriteConfig())
	, m_stage(stage)
{
}

void ComposeImpl::OnMouseRightDown(int x, int y)
{
	// 	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
	// 	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	// 
	// 	if (selected)
	// 	{
	// 		selected->setTransform(selected->getPosition(), selected->getAngle() + d2d::PI*0.5f);
	// 		m_stage->Refresh();
	// 	}

	//////////////////////////////////////////////////////////////////////////

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	const int col = pos.x / EDGE,
		row = pos.y / EDGE;
	d2d::ISprite* selected = m_stage->getSprite(row, col);
	if (selected)
	{
		selected->setTransform(selected->getPosition(), selected->getAngle() + d2d::PI*0.5f);
		m_stage->RefreshStage();
	}
}

}