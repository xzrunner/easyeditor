#ifndef _LIBRESPACKER_ANIM_FROM_LUA_H_
#define _LIBRESPACKER_ANIM_FROM_LUA_H_

#include "PackAnimation.h"

namespace librespacker
{

class AnimFromLua
{
public:
	static void Unpack(lua_State* L, PackAnimation* anim);

private:
	static void UnpackComponents(lua_State* L, PackAnimation* anim);
	static void UnpackFrames(lua_State* L, PackAnimation* anim);

	static void UnpackFrame(lua_State* L, PackAnimation::Frame& frame);
	static void UnpackPart(lua_State* L, PackAnimation::Part& part);

}; // AnimFromLua

}

#endif // _LIBRESPACKER_ANIM_FROM_LUA_H_