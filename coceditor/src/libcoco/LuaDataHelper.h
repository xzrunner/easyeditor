#ifndef _LIB_COCO_LUA_DATA_HELPER_H_
#define _LIB_COCO_LUA_DATA_HELPER_H_

#include <string>

struct lua_State;

namespace libcoco
{

class LuaDataHelper
{
public:
	static std::string GetStringField(lua_State* L, const char* name);
	static int GetIntField(lua_State* L, const char* name);

}; // LuaDataHelper

}

#endif // _LIB_COCO_LUA_DATA_HELPER_H_