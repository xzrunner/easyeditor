#ifndef _EASYCOCO_UTILITY_H_
#define _EASYCOCO_UTILITY_H_

#include <vector>
#include <map>

namespace ee { class Sprite; }
namespace s2 { class Sprite; }

namespace ecoco
{

class Utility
{
public:
	static void GroupSpritesFromTag(const std::vector<s2::Sprite*>& src, 
		std::map<std::string, std::vector<ee::Sprite*> >& dst, std::vector<ee::Sprite*>& others);

}; // Utility

}

#endif // _EASYCOCO_UTILITY_H_