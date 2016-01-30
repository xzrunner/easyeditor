#include "scripts.h"

namespace d2d
{
	extern "C" __declspec(dllexport) int luaopen_d2d(lua_State* L)
	{
		lua_newtable(L);
		// Preload module loaders.
		for (int i = 0; modules[i].name != 0; i++)
		{
			luax_preload(L, modules[i].func, modules[i].name);
		}
		return 1;
	}

	lua_State* scripts_create() 
	{
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);

		luax_preload(L, luaopen_d2d, "d2d");
		luaopen_d2d(L);

		return L;
	}

	void scripts_release(lua_State* L)
	{
		lua_close(L);
	}

	void scripts_do_string(const char* str)
	{
		lua_State* L = scripts_create();
		luaL_dostring(L, str);
		scripts_release(L);
	}

	void scripts_do_file(const char* filename)
	{
		lua_State* L = scripts_create();
		luaL_dofile(L, filename);
		scripts_release(L);
	}
}