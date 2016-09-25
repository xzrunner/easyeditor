#include "ComposeSkeletonOP.h"
#include "StagePanel.h"
#include "ComposeSkeletonImpl.h"

namespace eskeleton
{

ComposeSkeletonOP::ComposeSkeletonOP(StagePanel* stage, 
									 ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteOP<ee::SelectSpritesOP>(stage, stage->GetStageImpl(), 
	stage, property, NULL, ee::ArrangeSpriteConfig(), new ComposeSkeletonImpl(stage, property))
{
}

}