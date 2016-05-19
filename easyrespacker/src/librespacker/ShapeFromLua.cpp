#include "ShapeFromLua.h"
#include "PackShape.h"
#include "LuaDataHelper.h"

namespace erespacker
{

void ShapeFromLua::Unpack(lua_State* L, PackShape* shape)
{
	shape->type = ShapeType(LuaDataHelper::GetIntField(L, "shape_type"));
	shape->color.FromRGBA((uint32_t)LuaDataHelper::GetDoubleField(L, "color"));

	UnpackVertices(shape->vertices, L);
}

}