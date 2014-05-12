#ifndef D2D_WRAP_ISPRITE_H
#define D2D_WRAP_ISPRITE_H

#include <common/runtime.h>

#include "ISprite.h"

namespace d2d
{
	ISprite* luax_checksprite(lua_State* L, int idx);
	extern "C" int luaopen_sprite(lua_State* L);
}

#endif // D2D_WRAP_ISPRITE_H