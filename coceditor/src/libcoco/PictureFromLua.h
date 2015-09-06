#ifndef _LIBCOCO_PICTURE_FROM_LUA_H_
#define _LIBCOCO_PICTURE_FROM_LUA_H_

#include "PackPicture.h"

namespace libcoco
{

class PictureFromLua
{
public:
	static void Unpack(lua_State* L, const std::vector<d2d::Image*>& images,
		PackPicture* picture);

}; // PictureFromLua

}

#endif // _LIBCOCO_PICTURE_FROM_LUA_H_