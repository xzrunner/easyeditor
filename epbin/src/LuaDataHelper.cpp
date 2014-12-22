#include "LuaDataHelper.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

namespace epbin
{

std::string LuaDataHelper::GetStringField(lua_State* L, const char* name)
{
	lua_getfield(L, -1, "type");
	std::string type = lua_tostring(L, -1);
	lua_pop(L,1);
	return type;
}

int LuaDataHelper::GetIntField(lua_State* L, const char* name)
{
	lua_getfield(L, -1, "id");
	int id = lua_tointeger(L, -1);
	lua_pop(L,1);
	return id;
}

}