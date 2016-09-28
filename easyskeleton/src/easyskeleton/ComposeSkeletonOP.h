#ifndef _EASYSKELETON_COMPOSE_SKELETON_OP_H_
#define _EASYSKELETON_COMPOSE_SKELETON_OP_H_

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>

namespace eskeleton
{

class StagePanel;

class ComposeSkeletonOP : public ee::ArrangeSpriteOP<ee::SelectSpritesOP>
{
public:
	ComposeSkeletonOP(StagePanel* stage, ee::PropertySettingPanel* property);

}; // ComposeSkeletonOP

}

#endif // _EASYSKELETON_COMPOSE_SKELETON_OP_H_