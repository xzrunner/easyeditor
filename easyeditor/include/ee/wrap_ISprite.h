#ifndef _EASYEDITOR_WRAP_ISPRITE_H_
#define _EASYEDITOR_WRAP_ISPRITE_H_

extern "C" {
	#include <lua.h>
};

namespace ee
{

class Sprite;

Sprite* luax_checksprite(lua_State* L, int idx);
extern "C" int luaopen_sprite(lua_State* L);

}

#endif // _EASYEDITOR_WRAP_ISPRITE_H_