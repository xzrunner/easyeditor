#ifndef _EASYRESPACKER_PICTURE_FROM_LUA_H_
#define _EASYRESPACKER_PICTURE_FROM_LUA_H_

#include "PackPicture.h"

namespace erespacker
{

class PictureFromLua
{
public:
	static void Unpack(lua_State* L, const std::vector<ee::Image*>& images,
		PackPicture* picture);

}; // PictureFromLua

}

#endif // _EASYRESPACKER_PICTURE_FROM_LUA_H_