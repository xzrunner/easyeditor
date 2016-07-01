#ifndef _EASY_EASYCOMPLEX_GROUP_HELPER_H_
#define _EASY_EASYCOMPLEX_GROUP_HELPER_H_

#include <ee/Sprite.h>

#include <vector>

namespace ecomplex
{

class Sprite;

class GroupHelper
{
public:
	static Sprite* Group(const std::vector<ee::Sprite*>& sprites);
	static void BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprites);

}; // GroupHelper

}

#endif // _EASY_EASYCOMPLEX_GROUP_HELPER_H_