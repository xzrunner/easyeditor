#include "PasteSymbolOP.h"
#include "StagePanel.h"
#include "SelectSpritesOP.h"

namespace emap
{

PasteSymbolOP::PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library,
							 d2d::PropertySettingPanel* property)
	: d2d::PasteSymbolOP(stage, stage, library)
	, m_property(property)
{
}

// bool PasteSymbolOP::OnMouseRightDown(int x, int y)
// {
// 	if (d2d::PasteSymbolOP::OnMouseRightDown(x, y)) return true;
// 	ChangeCurrOP();
// 	return false;
// }

void PasteSymbolOP::ChangeCurrOP()
{
	StagePanel* stage = static_cast<StagePanel*>(m_stage);
	d2d::AbstractEditOP* editop = new d2d::ArrangeSpriteOP<SelectSpritesOP>(stage, stage, m_property);

	m_stage->SetEditOP(editop);	
}

}