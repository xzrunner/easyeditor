#ifndef _LIBRESPACKER_UTILITY_NEW_H_
#define _LIBRESPACKER_UTILITY_NEW_H_

#include <drag2d.h>

namespace librespacker
{

class Utility
{
public:
	static bool IsAnchor(const d2d::ISprite* spr);

	static bool IsNameValid(const std::string& name);

}; // Utility

}

#endif // _LIBRESPACKER_UTILITY_NEW_H_