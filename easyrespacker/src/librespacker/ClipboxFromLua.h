#ifndef _LIBRESPACKER_CLIPBOX_FROM_LUA_H_
#define _LIBRESPACKER_CLIPBOX_FROM_LUA_H_

#include "PackClipbox.h"

namespace librespacker
{

class ClipboxFromLua
{
public:
	static void Unpack(lua_State* L, PackClipbox* cb);

}; // ClipboxFromLua

}

#endif // _LIBRESPACKER_CLIPBOX_FROM_LUA_H_