#ifndef _EASYCOCO_UTILITY_H_
#define _EASYCOCO_UTILITY_H_

#include <drag2d.h>

namespace libcoco
{

class Utility
{
public:
	static void GroupSpritesFromTag(const std::vector<d2d::Sprite*>& src, 
		std::map<std::string, std::vector<d2d::Sprite*> >& dst, std::vector<d2d::Sprite*>& others);

}; // Utility

}

#endif // _EASYCOCO_UTILITY_H_