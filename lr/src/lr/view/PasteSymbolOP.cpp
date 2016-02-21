#include "PasteSymbolOP.h"
#include "StagePanel.h"
#include "SelectSpritesOP.h"

#include <ee/ArrangeSpriteOP.h>

namespace lr
{

PasteSymbolOP::PasteSymbolOP(StagePanel* stage, ee::LibraryPanel* library,
							 ee::PropertySettingPanel* property)
	: ee::PasteSymbolOP(stage, stage->GetStageImpl(), library)
	, m_property(property)
{
}

void PasteSymbolOP::ChangeCurrOP()
{
	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	ee::EditOP* editop = new ee::ArrangeSpriteOP<SelectSpritesOP>(
		stage, stage->GetStageImpl(), stage, m_property);

	m_stage->SetEditOP(editop);	
}

}