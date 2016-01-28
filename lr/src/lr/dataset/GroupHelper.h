#ifndef _LR_GROUP_HELPER_H_
#define _LR_GROUP_HELPER_H_

#include <drag2d.h>

namespace lr
{

class GroupHelper
{
public:
	static d2d::ISprite* Group(const std::vector<d2d::ISprite*>& sprites);
	static void BreakUp(d2d::ISprite* group, std::vector<d2d::ISprite*>& sprites);

}; // GroupHelper

}

#endif // _LR_GROUP_HELPER_H_