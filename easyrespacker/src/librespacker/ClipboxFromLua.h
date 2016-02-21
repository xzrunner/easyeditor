#ifndef _EASYRESPACKER_CLIPBOX_FROM_LUA_H_
#define _EASYRESPACKER_CLIPBOX_FROM_LUA_H_

#include "PackClipbox.h"

namespace erespacker
{

class ClipboxFromLua
{
public:
	static void Unpack(lua_State* L, PackClipbox* cb);

}; // ClipboxFromLua

}

#endif // _EASYRESPACKER_CLIPBOX_FROM_LUA_H_