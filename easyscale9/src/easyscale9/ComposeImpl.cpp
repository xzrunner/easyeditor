//#include "ComposeImpl.h"
//#include "StagePanel.h"
//#include "ComposeGrids.h"
//
//namespace escale9
//{
//
//ComposeImpl::ComposeImpl(StagePanel* stage, ee::PropertySettingPanel* property)
//	: ee::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property, NULL, 
//	ee::ArrangeSpriteConfig(false, false, false, false))
//	, m_stage(stage)
//{
//}
//
//void ComposeImpl::OnMouseRightDown(int x, int y)
//{
//	// 	ee::Vector pos = m_stage->transPosScreenToProject(x, y);
//	// 	ee::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
//	// 
//	// 	if (selected)
//	// 	{
//	// 		selected->setTransform(selected->getPosition(), selected->getAngle() + ee::PI*0.5f);
//	// 		m_stage->Refresh();
//	// 	}
//
//	//////////////////////////////////////////////////////////////////////////
//
//	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
//	int col, row;
//	ComposeGrids::Query(pos, &col, &row);
//	if (col == -1 || row == -1) {
//		return;
//	}
//
//	ee::ISprite* selected = m_stage->getSprite(row, col);
//	if (selected)
//	{
//		selected->SetTransform(selected->GetPosition(), selected->GetAngle() + ee::PI*0.5f);
//		m_stage->SetCanvasDirty();
//	}
//}
//
//}