#include "TrailFromLua.h"
#include "UnpackNodeFactory.h"
#include "LuaDataHelper.h"

#include <mt_2d.h>

#include <assert.h>

namespace erespacker
{

void TrailFromLua::Unpack(lua_State* L, PackTrail* trail)
{
	trail->comp_images.clear();
	trail->comp_shapes.clear();

	if (LuaDataHelper::HasField(L, "export")) {
		trail->export_name = LuaDataHelper::GetStringField(L, "export");
	}

	UnpackBody(L, trail);
	UnpackComponents(L, trail);
}

void TrailFromLua::UnpackBody(lua_State* L, PackTrail* trail)
{
	trail->mode = LuaDataHelper::GetIntField(L, "mode");

	trail->count = LuaDataHelper::GetIntField(L, "count");

	trail->life_begin = TransTime(LuaDataHelper::GetIntField(L, "life_begin"));
	trail->life_offset = TransTime(LuaDataHelper::GetIntField(L, "life_offset"));

	trail->fadeout_time = TransTime(LuaDataHelper::GetIntField(L, "fadeout_time"));
}

void TrailFromLua::UnpackComponents(lua_State* L, PackTrail* trail)
{
	lua_getfield(L, -1, "component");
	int comp_sz = lua_rawlen(L, -1);

	if (trail->mode == T2D_MODE_IMAGE) {
		UnpackNodeFactory* factory = UnpackNodeFactory::Instance();
		trail->comp_images.reserve(comp_sz);
		for (int i = 1; i <= comp_sz; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			assert(lua_istable(L, -1));

			trail->comp_images.push_back(PackTrail::CompImage());
			PackTrail::CompImage& comp = trail->comp_images[trail->comp_images.size() - 1];

			int id = LuaDataHelper::GetIntField(L, "id");
			comp.node = factory->Query(id);
			if (!comp.node) {
				factory->AddUnassigned(id, &comp.node);
			}

			comp.scale_begin = TransFloatX100(LuaDataHelper::GetIntField(L, "scale_begin"));
			comp.scale_end = TransFloatX100(LuaDataHelper::GetIntField(L, "scale_end"));

			comp.mul_col_begin = (uint32_t)LuaDataHelper::GetDoubleField(L, "mul_col_begin");
			comp.mul_col_end = (uint32_t)LuaDataHelper::GetDoubleField(L, "mul_col_end");
			comp.add_col_begin = (uint32_t)LuaDataHelper::GetDoubleField(L, "add_col_begin");
			comp.add_col_end = (uint32_t)LuaDataHelper::GetDoubleField(L, "add_col_end");

			lua_pop(L, 1);
		}
	} else {
		assert(trail->mode == T2D_MODE_SHAPE);
		trail->comp_shapes.reserve(comp_sz);
		for (int i = 1; i < comp_sz; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			assert(lua_istable(L, -1));

			PackTrail::CompShape comp;

			comp.linewidth = (float)LuaDataHelper::GetDoubleField(L, "linewidth");
			comp.acuity = TransFloatX100(LuaDataHelper::GetIntField(L, "acuity"));

			comp.col_begin = (uint32_t)LuaDataHelper::GetDoubleField(L, "col_begin");
			comp.col_end = (uint32_t)LuaDataHelper::GetDoubleField(L, "col_end");

			trail->comp_shapes.push_back(comp);

			lua_pop(L, 1);
		}
	}

	lua_pop(L, 1);
}

}