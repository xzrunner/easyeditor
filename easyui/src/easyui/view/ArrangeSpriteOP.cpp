#include "ArrangeSpriteOP.h"
#include "StagePanel.h"

namespace eui
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage, d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteOP<SelectSpritesOP>(stage, stage, property)
{
}

bool ArrangeSpriteOP::onDraw() const
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onDraw()) {
		return true;
	}

	

	return false;
}

}