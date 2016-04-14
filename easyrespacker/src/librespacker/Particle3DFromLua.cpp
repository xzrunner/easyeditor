#include "Particle3DFromLua.h"
#include "UnpackNodeFactory.h"
#include "LuaDataHelper.h"

#include <assert.h>

namespace erespacker
{

void Particle3DFromLua::Unpack(lua_State* L, PackParticle3D* p3d)
{
	p3d->components.clear();

	if (LuaDataHelper::HasField(L, "export")) {
		p3d->export_name = LuaDataHelper::GetStringField(L, "export");
	}

	UnpackComponents(L, p3d);
	UnpackBody(L, p3d);
}

void Particle3DFromLua::UnpackComponents(lua_State* L, PackParticle3D* p3d)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	lua_getfield(L, -1, "component");
	int comp_sz = lua_rawlen(L, -1);
	p3d->components.reserve(comp_sz);
	for (int i = 1; i <= comp_sz; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		p3d->components.push_back(PackParticle3D::Component());
		PackParticle3D::Component& comp = p3d->components[p3d->components.size() - 1];

		int id = LuaDataHelper::GetIntField(L, "id");
		comp.node = factory->Query(id);
		if (!comp.node) {
			factory->AddUnassigned(id, &comp.node);
		}

		comp.count = LuaDataHelper::GetIntField(L, "count");

		comp.scale_start = TransFloatX100(LuaDataHelper::GetIntField(L, "scale_start"));
		comp.scale_end = TransFloatX100(LuaDataHelper::GetIntField(L, "scale_end"));

		comp.angle = static_cast<float>(LuaDataHelper::GetIntField(L, "angle"));
		comp.angle_var = static_cast<float>(LuaDataHelper::GetIntField(L, "angle_var"));

		comp.col_mul = (uint32_t)LuaDataHelper::GetDoubleField(L, "col_mul");
		comp.col_add = (uint32_t)LuaDataHelper::GetDoubleField(L, "col_add");
		comp.alpha_start = (uint8_t)LuaDataHelper::GetIntField(L, "alpha_start");
		comp.alpha_end = (uint8_t)LuaDataHelper::GetIntField(L, "alpha_end");

		lua_pop(L, 1);
	}
	lua_pop(L, 1);	
}

void Particle3DFromLua::UnpackBody(lua_State* L, PackParticle3D* p3d)
{
	p3d->static_mode = LuaDataHelper::GetBoolField(L, "static_mode");

	p3d->emission_time = TransTime(LuaDataHelper::GetIntField(L, "emission_time"));
	p3d->count = LuaDataHelper::GetIntField(L, "count");

	p3d->life = TransTime(LuaDataHelper::GetIntField(L, "life"));
	p3d->life_var = TransTime(LuaDataHelper::GetIntField(L, "life_var"));

	p3d->hori = TransDegree(LuaDataHelper::GetIntField(L, "hori"));
	p3d->hori_var = TransDegree(LuaDataHelper::GetIntField(L, "hori_var"));
	p3d->vert = TransDegree(LuaDataHelper::GetIntField(L, "vert"));
	p3d->vert_var = TransDegree(LuaDataHelper::GetIntField(L, "vert_var"));

	p3d->radial_spd = static_cast<float>(LuaDataHelper::GetIntField(L, "radial_spd"));
	p3d->radial_spd_var = static_cast<float>(LuaDataHelper::GetIntField(L, "radial_spd_var"));
	p3d->tangential_spd = static_cast<float>(LuaDataHelper::GetIntField(L, "tangential_spd"));
	p3d->tangential_spd_var = static_cast<float>(LuaDataHelper::GetIntField(L, "tangential_spd_var"));
	p3d->angular_spd = TransDegree(LuaDataHelper::GetIntField(L, "angular_spd"));
	p3d->angular_spd_var = TransDegree(LuaDataHelper::GetIntField(L, "angular_spd_var"));

	p3d->dis_region = static_cast<float>(LuaDataHelper::GetIntField(L, "dis_region"));
	p3d->dis_region_var = static_cast<float>(LuaDataHelper::GetIntField(L, "dis_region_var"));
	p3d->dis_spd = static_cast<float>(LuaDataHelper::GetIntField(L, "dis_spd"));
	p3d->dis_spd_var = static_cast<float>(LuaDataHelper::GetIntField(L, "dis_spd_var"));

	p3d->gravity = static_cast<float>(LuaDataHelper::GetIntField(L, "gravity"));

	p3d->linear_acc = static_cast<float>(LuaDataHelper::GetIntField(L, "linear_acc"));
	p3d->linear_acc_var = static_cast<float>(LuaDataHelper::GetIntField(L, "linear_acc_var"));

	p3d->fadeout_time = TransTime(LuaDataHelper::GetIntField(L, "fadeout_time"));

	p3d->ground = LuaDataHelper::GetIntField(L, "ground");

	p3d->start_radius = static_cast<float>(LuaDataHelper::GetIntField(L, "start_radius"));
	p3d->start_height = static_cast<float>(LuaDataHelper::GetIntField(L, "start_height"));

	p3d->orient_to_movement = LuaDataHelper::GetBoolField(L, "orient_to_movement");
}

}