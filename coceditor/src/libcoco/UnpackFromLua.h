#ifndef _LIBCOCO_COCO_UNPACKE_FROM_LUA_H_
#define _LIBCOCO_COCO_UNPACKE_FROM_LUA_H_

#include <drag2d.h>

namespace libcoco
{

class UnpackFromLua
{
public:
	static void Unpack(const std::string& filepath, 
		const std::vector<d2d::Image*>& images);

}; // UnpackFromLua

}

#endif // _LIBCOCO_COCO_UNPACKE_FROM_LUA_H_