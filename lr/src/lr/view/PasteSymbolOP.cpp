#include "PasteSymbolOP.h"
#include "StagePanel.h"
#include "SelectSpritesOP.h"

namespace lr
{

PasteSymbolOP::PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library,
							 d2d::PropertySettingPanel* property)
	: d2d::PasteSymbolOP(stage, stage->GetStageImpl(), library)
	, m_property(property)
{
}

void PasteSymbolOP::ChangeCurrOP()
{
	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	d2d::AbstractEditOP* editop = new d2d::ArrangeSpriteOP<SelectSpritesOP>(
		stage, stage->GetStageImpl(), stage, m_property);

	m_stage->SetEditOP(editop);	
}

}