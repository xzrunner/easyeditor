#ifndef _LR_GROUP_HELPER_H_
#define _LR_GROUP_HELPER_H_

#include <vector>

namespace ee { class Sprite; }

namespace lr
{

class GroupHelper
{
public:
	static ee::Sprite* Group(const std::vector<ee::Sprite*>& sprites, const std::string& name);
	static void BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprites);

}; // GroupHelper

}

#endif // _LR_GROUP_HELPER_H_