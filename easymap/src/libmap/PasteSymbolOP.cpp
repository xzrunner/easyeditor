#include "PasteSymbolOP.h"
#include "StagePanel.h"

#include <easycomplex.h>

namespace emap
{

PasteSymbolOP::PasteSymbolOP(StagePanel* stage, ee::LibraryPanel* library,
							 ee::PropertySettingPanel* property)
	: ee::PasteSymbolOP(stage, stage->GetStageImpl(), stage, library)
	, m_property(property)
{
}

// bool PasteSymbolOP::OnMouseRightDown(int x, int y)
// {
// 	if (ee::PasteSymbolOP::OnMouseRightDown(x, y)) return true;
// 	ChangeCurrOP();
// 	return false;
// }

void PasteSymbolOP::ChangeCurrOP()
{
	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	ee::EditOP* editop = new ee::ArrangeSpriteOP<ecomplex::SelectSpritesOP>(stage, stage->GetStageImpl(), stage, m_property);

	m_stage->SetEditOP(editop);	
}

}