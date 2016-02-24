#include "ArrangeSpriteOP.h"
#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"

namespace etexpacker
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* editPanel, 
	ee::PropertySettingPanel* property,
	ee::EditCMPT* callback/* = NULL*/)
	: ee::ArrangeSpriteOP<ee::SelectSpritesOP>(editPanel, editPanel->GetStageImpl(), editPanel, property, 
	callback, ee::ArrangeSpriteConfig(false, false, false, false), 
	new ArrangeSpriteImpl(editPanel, property))
{
}

}