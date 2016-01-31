#include "ArrangeSpriteOP.h"
#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"

namespace etexpacker
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* editPanel, 
	ee::PropertySettingPanel* propertyPanel,
	ee::EditCMPT* callback/* = NULL*/)
	: ee::ArrangeSpriteOP<ee::SelectSpritesOP>(editPanel, editPanel->GetStageImpl(), editPanel, propertyPanel, 
	callback, ee::ArrangeSpriteConfig(false, false, false, false), 
	new ArrangeSpriteImpl(editPanel, propertyPanel))
{
}


}