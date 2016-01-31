#ifndef _EASYANIM_SKELETON_OP_H_
#define _EASYANIM_SKELETON_OP_H_

#include "SelectSpritesOP.h"

namespace eanim
{

class StagePanel;

class SkeletonOP : public ee::ArrangeSpriteOP<SelectSpritesOP>
{
public:
	SkeletonOP(StagePanel* stage, ee::PropertySettingPanel* property);

}; // SkeletonOP

}

#endif // _EASYANIM_SKELETON_OP_H_