#include "ClipboxFromLua.h"
#include "LuaDataHelper.h"

namespace librespacker
{

void ClipboxFromLua::Unpack(lua_State* L, PackClipbox* cb)
{
	cb->x = LuaDataHelper::GetIntField(L, "x");
	cb->y = LuaDataHelper::GetIntField(L, "y");
	cb->w = LuaDataHelper::GetIntField(L, "w");
	cb->h = LuaDataHelper::GetIntField(L, "h");
}

}