#include "ComposeImpl.h"
#include "StagePanel.h"
#include "ComposeGrids.h"

namespace escale9
{

ComposeImpl::ComposeImpl(StagePanel* stage, d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property, NULL, 
	d2d::ArrangeSpriteConfig(false, false, false, false))
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
	int col, row;
	ComposeGrids::Query(pos, &col, &row);
	if (col == -1 || row == -1) {
		return;
	}

	d2d::ISprite* selected = m_stage->getSprite(row, col);
	if (selected)
	{
		selected->SetTransform(selected->GetPosition(), selected->GetAngle() + d2d::PI*0.5f);
		m_stage->SetCanvasDirty();
	}
}

}