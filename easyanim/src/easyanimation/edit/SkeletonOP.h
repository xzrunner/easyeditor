#ifndef _EASYANIM_SKELETON_OP_H_
#define _EASYANIM_SKELETON_OP_H_

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

#endif // _EASYANIM_SKELETON_OP_H_