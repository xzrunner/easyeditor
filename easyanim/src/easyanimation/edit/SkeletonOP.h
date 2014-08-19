#ifndef EANIM_SKELETON_OP_H
#define EANIM_SKELETON_OP_H

#include "SelectSpritesOP.h"

namespace eanim
{

class StagePanel;

class SkeletonOP : public d2d::ArrangeSpriteOP<SelectSpritesOP>
{
public:
	SkeletonOP(StagePanel* stage, d2d::PropertySettingPanel* property);

}; // SkeletonOP

}

#endif // EANIM_SKELETON_OP_H