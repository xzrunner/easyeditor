#ifndef D2D_SCRIPTS_H
#define D2D_SCRIPTS_H

#include "view/wrap_StagePanel.h"

namespace d2d
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

#endif // D2D_SCRIPTS_H