#ifndef _EASYCOCO_UTILITY_H_
#define _EASYCOCO_UTILITY_H_

#include <ee/Sprite.h>

#include <vector>
#include <map>

namespace s2 { class Sprite; }

namespace ecoco
{

class Utility
{
public:
	static void GroupSpritesFromTag(const CU_VEC<s2::SprPtr>& src,
		std::map<std::string, std::vector<ee::SprPtr> >& dst, std::vector<ee::SprPtr>& others);

}; // Utility

}

#endif // _EASYCOCO_UTILITY_H_