#include "ArrangeSpriteOP.h"
#include "StagePanel.h"

namespace eui
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteOP<SelectSpritesOP>(stage, stage, property)
{
}

bool ArrangeSpriteOP::onDraw() const
{
	if (ee::ArrangeSpriteOP<SelectSpritesOP>::onDraw()) {
		return true;
	}

	

	return false;
}

}