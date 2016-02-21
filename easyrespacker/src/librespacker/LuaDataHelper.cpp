#include "LuaDataHelper.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace erespacker
{

std::string LuaDataHelper::GetStringField(lua_State* L, const char* name)
{
	lua_getfield(L, -1, name);
	std::string type = lua_tostring(L, -1);
	lua_pop(L,1);
	return type;
}

int LuaDataHelper::GetIntField(lua_State* L, const char* name)
{
	lua_getfield(L, -1, name);
	int id = lua_tointeger(L, -1);
	lua_pop(L,1);
	return id;
}

double LuaDataHelper::GetDoubleField(lua_State* L, const char* name)
{
	lua_getfield(L, -1, name);
	double id = lua_tonumber(L, -1);
	lua_pop(L,1);
	return id;	
}

bool LuaDataHelper::GetBoolField(lua_State* L, const char* name)
{
	lua_getfield(L, -1, name);
	bool ret = (bool)lua_toboolean(L, -1);
	lua_pop(L, 1);
	return ret;
}

bool LuaDataHelper::HasField(lua_State* L, const char* field)
{
	lua_getfield(L, -1, field);
	bool has_field = lua_type(L, -1) != LUA_TNIL;
	lua_pop(L, 1);
	return has_field;
}

}