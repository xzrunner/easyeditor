#include "ArrangeSpriteOP.h"
#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"

namespace etexpacker
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* editPanel, 
	d2d::PropertySettingPanel* propertyPanel,
	d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(editPanel, editPanel->GetStageImpl(), editPanel, propertyPanel, 
	callback, d2d::ArrangeSpriteConfig(false, false, false, false), 
	new ArrangeSpriteImpl(editPanel, propertyPanel))
{
}


}