#include "AnimFromLua.h"
#include "UnpackNodeFactory.h"
#include "spritepack.h"

#include <epbin.h>

namespace librespacker
{

void AnimFromLua::Unpack(lua_State* L, PackAnimation* anim)
{
	anim->Clear();

	if (epbin::LuaDataHelper::HasField(L, "export")) {
		anim->export_name = epbin::LuaDataHelper::GetStringField(L, "export");
	}

	UnpackComponents(L, anim);
	UnpackFrames(L, anim);
}

void AnimFromLua::UnpackComponents(lua_State* L, PackAnimation* anim)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	lua_getfield(L, -1, "component");
	int comp_sz = lua_rawlen(L, -1);
	anim->components.reserve(comp_sz);
	for (int i = 1; i <= comp_sz; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		anim->components.push_back(PackAnimation::Component());
		PackAnimation::Component& comp = anim->components[anim->components.size() - 1];
		int id;
		if (epbin::LuaDataHelper::HasField(L, "id")) {
			id = epbin::LuaDataHelper::GetIntField(L, "id");
		} else {
			id = ANCHOR_ID;
		}
		comp.node = factory->Query(id);
		if (!comp.node) {
			factory->AddUnassigned(id, &comp.node);
		}
		if (epbin::LuaDataHelper::HasField(L, "name")) {
			comp.name = epbin::LuaDataHelper::GetStringField(L, "name");
		}

		lua_pop(L, 1);
	}
	lua_pop(L, 1);	
}

void AnimFromLua::UnpackFrames(lua_State* L, PackAnimation* anim)
{
	int action_sz = lua_rawlen(L, -1);
	for (int i = 1; i <= action_sz; ++i)
	{
		PackAnimation::Action action;

		if (epbin::LuaDataHelper::HasField(L, "name")) {
			action.name = epbin::LuaDataHelper::GetStringField(L, "action");
		}

		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));
		int frame_sz = lua_rawlen(L, -1);
		action.size = frame_sz;
		for (int j = 1; j <= frame_sz; ++j) {
			lua_pushinteger(L, j);
			lua_gettable(L, -2);
			assert(lua_istable(L, -1));

			PackAnimation::Frame frame;
			UnpackFrame(L, frame);
			anim->frames.push_back(frame);

			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		anim->actions.push_back(action);
	}
}

void AnimFromLua::UnpackFrame(lua_State* L, PackAnimation::Frame& frame)
{
	int spr_sz = lua_rawlen(L, -1);
	for (int i = 1; i <= spr_sz; ++i) 
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);

		PackAnimation::Part part;
		UnpackPart(L, part);
		frame.parts.push_back(part);

		lua_pop(L, 1);
	}
}

void AnimFromLua::UnpackPart(lua_State* L, PackAnimation::Part& part)
{
	if (lua_isnumber(L, -1)) {
		part.comp_idx = lua_tointeger(L, -1);
	} else if (lua_istable(L, -1)) {
		part.comp_idx = epbin::LuaDataHelper::GetIntField(L, "index");
		if (epbin::LuaDataHelper::HasField(L, "mat")) {
			lua_getfield(L, -1, "mat");
			assert(lua_type(L, -1) != LUA_TNIL);
			int len = lua_rawlen(L, -1);
			assert(len == 6);
			for (int i = 1; i <= len; ++i)
			{
				lua_pushinteger(L, i);
				lua_gettable(L, -2);
				part.t.mat[i - 1] = lua_tointeger(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		}
		if (epbin::LuaDataHelper::HasField(L, "color")) {
			part.t.color = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "color");
		}
		if (epbin::LuaDataHelper::HasField(L, "add")) {
			part.t.additive = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "add");
		}
		if (epbin::LuaDataHelper::HasField(L, "rmap")) {
			part.t.rmap = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "rmap");
		}
		if (epbin::LuaDataHelper::HasField(L, "gmap")) {
			part.t.gmap = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "gmap");
		}
		if (epbin::LuaDataHelper::HasField(L, "bmap")) {
			part.t.bmap = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "bmap");
		}
	} else {
		throw d2d::Exception("PackAnimation::UnpackFromLua unknown item type.");
	}
}

}