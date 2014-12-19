#include "LuaDataHelper.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

namespace epbin
{

LuaDataHelper::LuaDataHelper(lua_State* L)
	: L(L)
{
}

std::string LuaDataHelper::GetStringField(const char* name)
{
	lua_getfield(L, -1, "type");
	std::string type = lua_tostring(L, -1);
	lua_pop(L,1);
	return type;
}

int LuaDataHelper::GetIntField(const char* name)
{
	lua_getfield(L, -1, "id");
	int id = lua_tointeger(L, -1);
	lua_pop(L,1);
	return id;
}

}