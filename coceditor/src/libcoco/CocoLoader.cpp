#include "CocoLoader.h"
#include "LuaDataHelper.h"

#define STRINGIFY(A)  #A
#include "trans_ejoy2d_data.lua"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <iostream>

namespace libcoco
{

void CocoLoader::Load(const std::string& filepath)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_settop(L,0);

	int err = luaL_dostring(L, trans_ejoy2d_data);
	if (err) {
		const char *msg = lua_tostring(L,-1);
		std::cout << msg << std::endl;
	}
	lua_getglobal(L, "trans2table");
	lua_pushstring(L, filepath.c_str());
	err = lua_pcall(L, 1, 1, 0);
	if (err) {
		const char *msg = lua_tostring(L,-1);
		std::cout << msg << std::endl;
	}

	if (lua_gettop(L) != 1 || !lua_istable(L, 1)) {
		return;
	}

	int len = lua_rawlen(L, 1);
	for(int i = 1; i <= len; i++)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, 1);

		if (lua_istable(L, -1))
		{
// 			std::string type = LuaDataHelper::GetStringField(L, "type");
// 			int id = LuaDataHelper::GetIntField(L, "id");
// 			CheckID(id);
// 			CheckExport(L);
// 			if (type == "picture") {
// 				m_pictures.push_back(new epd::Picture(L, id));
// 			} else if (type == "animation") {
// 				m_animations.push_back(new epd::Animation(L, id));
// 			} else {
// 				assert(0);
// 			}
		}
		lua_pop(L,1);
	}
}

}