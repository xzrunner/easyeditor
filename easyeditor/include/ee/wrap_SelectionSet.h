#ifndef _EASYEDITOR_WRAP_SELECTION_SET_H_
#define _EASYEDITOR_WRAP_SELECTION_SET_H_

#include "runtime.h"

namespace ee
{

class SpriteSelection;

SpriteSelection* luax_checkSpriteSelection(lua_State* L, int idx);
extern "C" int luaopen_SpriteSelection(lua_State* L);

}

#endif // _EASYEDITOR_WRAP_SELECTION_SET_H_