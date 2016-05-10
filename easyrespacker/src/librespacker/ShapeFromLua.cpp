#include "ShapeFromLua.h"
#include "LuaDataHelper.h"
#include "typedef.h"

#include <assert.h>

namespace erespacker
{

void ShapeFromLua::Unpack(lua_State* L, PackShape* shape)
{
	shape->type = ShapeType(LuaDataHelper::GetIntField(L, "shape_type"));
	shape->color.FromRGBA((uint32_t)LuaDataHelper::GetDoubleField(L, "color"));

	int num = LuaDataHelper::GetIntField(L, "vertices_num");
	shape->vertices.resize(num);
	lua_getfield(L, -1, "vertices");
	int len = lua_rawlen(L, -1);
	assert(len == num * 2);
	for (int i = 0; i <= len; ++i) 
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		int screen = static_cast<int>(lua_tonumber(L, -1));
		if (i % 2) {
			shape->vertices[(i - 1) / 2].x = screen / SCALE;
		} else {
			shape->vertices[(i - 1) / 2].y =-screen / SCALE;
		}
		lua_pop(L, 1);
	}
}

}