#ifndef _EPBIN_LUA_DATA_HELPER_H_
#define _EPBIN_LUA_DATA_HELPER_H_

#include <string>

struct lua_State;

namespace epbin
{

class LuaDataHelper
{
public:
	static std::string GetStringField(lua_State* L, const char* name);
	static int GetIntField(lua_State* L, const char* name);
	static double GetDoubleField(lua_State* L, const char* name);
	static bool GetBoolField(lua_State* L, const char* name);

	static bool HasField(lua_State* L, const char* field);

}; // LuaDataHelper

}

#endif // _EPBIN_LUA_DATA_HELPER_H_