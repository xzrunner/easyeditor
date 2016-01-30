#include "ShapeFromLua.h"
#include "LuaDataHelper.h"
#include "typedef.h"

namespace librespacker
{

void ShapeFromLua::Unpack(lua_State* L, PackShape* shape)
{
	shape->type = ShapeType(LuaDataHelper::GetIntField(L, "shape_type"));
	shape->color = d2d::TransColor((uint32_t)LuaDataHelper::GetDoubleField(L, "color"), d2d::PT_RGBA);

	int num = LuaDataHelper::GetIntField(L, "vertices_num");
	shape->vertices.resize(num);
	lua_getfield(L, -1, "vertices");
	int len = lua_rawlen(L, -1);
	assert(len == num * 2);
	for (int i = i; i <= len; ++i) 
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		int screen = lua_tonumber(L, -1);
		if (i % 2) {
			shape->vertices[(i - 1) / 2].x = screen / SCALE;
		} else {
			shape->vertices[(i - 1) / 2].y =-screen / SCALE;
		}
		lua_pop(L, 1);
	}
}

}