#ifndef D2D_WRAP_SELECTION_SET_H
#define D2D_WRAP_SELECTION_SET_H

#include <common/runtime.h>

#include "SelectionSet.h"

namespace d2d
{
	SpriteSelection* luax_checkSpriteSelection(lua_State* L, int idx);
	extern "C" int luaopen_SpriteSelection(lua_State* L);
}

#endif // D2D_WRAP_SELECTION_SET_H