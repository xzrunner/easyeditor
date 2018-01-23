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
	m_stage->SetEditOP(std::make_shared<ee::ArrangeSpriteOP<SelectSpritesOP>>(
		stage, stage->GetStageImpl(), stage, m_property));
}

}