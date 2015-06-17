#include "SkeletonOP.h"
#include "SkeletonImpl.h"

#include "view/StagePanel.h"

namespace eanim
{

SkeletonOP::SkeletonOP(StagePanel* stage, d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteOP<SelectSpritesOP>(stage, stage, property, NULL, NULL, d2d::ArrangeSpriteConfig(), 
	new SkeletonImpl(stage, property, this))
{
}

}