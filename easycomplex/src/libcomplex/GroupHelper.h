#ifndef _EASYCOMPLEX_GROUP_HELPER_H_
#define _EASYCOMPLEX_GROUP_HELPER_H_

#include <ee/Sprite.h>

#include <vector>

namespace ecomplex
{

class Sprite;

class GroupHelper
{
public:
	static Sprite* Group(const std::vector<ee::Sprite*>& sprs);
	static void BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprs);

}; // GroupHelper

}

#endif // _EASYCOMPLEX_GROUP_HELPER_H_