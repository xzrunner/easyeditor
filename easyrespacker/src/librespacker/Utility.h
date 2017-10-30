#ifndef _EASYRESPACKER_UTILITY_NEW_H_
#define _EASYRESPACKER_UTILITY_NEW_H_

#include <ee/Sprite.h>

#include <string>

namespace erespacker
{

class Utility
{
public:
	static bool IsAnchor(const ee::SprConstPtr& spr);

	static bool IsNameValid(const std::string& name);

}; // Utility

}

#endif // _EASYRESPACKER_UTILITY_NEW_H_