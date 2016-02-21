#ifndef _EASYRESPACKER_UTILITY_NEW_H_
#define _EASYRESPACKER_UTILITY_NEW_H_

#include <string>

namespace ee { class Sprite; }

namespace erespacker
{

class Utility
{
public:
	static bool IsAnchor(const ee::Sprite* spr);

	static bool IsNameValid(const std::string& name);

}; // Utility

}

#endif // _EASYRESPACKER_UTILITY_NEW_H_