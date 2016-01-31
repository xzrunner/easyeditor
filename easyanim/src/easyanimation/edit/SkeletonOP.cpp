#include "SkeletonOP.h"
#include "SkeletonImpl.h"

#include "view/StagePanel.h"

namespace eanim
{

SkeletonOP::SkeletonOP(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteOP<SelectSpritesOP>(stage, stage->GetStageImpl(), stage, property, 
	NULL, ee::ArrangeSpriteConfig(), new SkeletonImpl)
{
}

}