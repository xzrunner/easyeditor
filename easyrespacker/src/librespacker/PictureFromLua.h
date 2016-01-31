#ifndef _LIBRESPACKER_PICTURE_FROM_LUA_H_
#define _LIBRESPACKER_PICTURE_FROM_LUA_H_

#include "PackPicture.h"

namespace librespacker
{

class PictureFromLua
{
public:
	static void Unpack(lua_State* L, const std::vector<ee::Image*>& images,
		PackPicture* picture);

}; // PictureFromLua

}

#endif // _LIBRESPACKER_PICTURE_FROM_LUA_H_