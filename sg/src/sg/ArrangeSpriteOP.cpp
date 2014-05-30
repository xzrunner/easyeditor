#include "ArrangeSpriteOP.h"
#include "StagePanel.h"

namespace sg
{

ArrangeSpriteOP::ArrangeSpriteOP(StagePanel* stage, d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteFixOP(stage, stage, property)
{
}

}