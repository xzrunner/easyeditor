#ifndef _LR_GROUP_HELPER_H_
#define _LR_GROUP_HELPER_H_

#include <drag2d.h>

namespace lr
{

class GroupHelper
{
public:
	static d2d::Sprite* Group(const std::vector<d2d::Sprite*>& sprites);
	static void BreakUp(d2d::Sprite* group, std::vector<d2d::Sprite*>& sprites);

}; // GroupHelper

}

#endif // _LR_GROUP_HELPER_H_