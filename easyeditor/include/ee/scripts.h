#ifndef _EASYEDITOR_SCRIPTS_H_
#define _EASYEDITOR_SCRIPTS_H_

#include "wrap_StagePanel.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

namespace ee
{

static const luaL_Reg modules[] = {
	{ "d2d.stage", luaopen_d2d_stage },
	{ 0, 0 }
};

extern "C" __declspec(dllexport) int luaopen_d2d(lua_State* L);

lua_State* scripts_create();
void scripts_release(lua_State* L);

void scripts_do_string(const char* str);
void scripts_do_file(const char* filename);

}

#endif // _EASYEDITOR_SCRIPTS_H_