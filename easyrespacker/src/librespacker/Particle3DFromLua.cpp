#include "Particle3DFromLua.h"
#include "UnpackNodeFactory.h"
#include "LuaDataHelper.h"

namespace librespacker
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

		comp.scale_start = TransFloatX100(LuaDataHelper::GetIntField(L, "scale_start"));
		comp.scale_end = TransFloatX100(LuaDataHelper::GetIntField(L, "scale_end"));

		comp.angle = LuaDataHelper::GetIntField(L, "angle");
		comp.angle_var = LuaDataHelper::GetIntField(L, "angle_var");

		comp.col_mul = (uint32_t)LuaDataHelper::GetDoubleField(L, "col_mul");
		comp.col_add = (uint32_t)LuaDataHelper::GetDoubleField(L, "col_add");
		comp.alpha_start = TransFloatX100(LuaDataHelper::GetIntField(L, "alpha_start"));
		comp.alpha_end = TransFloatX100(LuaDataHelper::GetIntField(L, "alpha_end"));

		lua_pop(L, 1);
	}
	lua_pop(L, 1);	
}

void Particle3DFromLua::UnpackBody(lua_State* L, PackParticle3D* p3d)
{
	p3d->capacity = LuaDataHelper::GetIntField(L, "capacity");

	p3d->emission_time = TransTime(LuaDataHelper::GetIntField(L, "emission_time"));
	p3d->count = LuaDataHelper::GetIntField(L, "count");

	p3d->life = TransTime(LuaDataHelper::GetIntField(L, "life"));
	p3d->life_var = TransTime(LuaDataHelper::GetIntField(L, "life_var"));

	p3d->hori = TransDegree(LuaDataHelper::GetIntField(L, "hori"));
	p3d->hori_var = TransDegree(LuaDataHelper::GetIntField(L, "hori_var"));
	p3d->vert = TransDegree(LuaDataHelper::GetIntField(L, "vert"));
	p3d->vert_var = TransDegree(LuaDataHelper::GetIntField(L, "vert_var"));

	p3d->spd = LuaDataHelper::GetIntField(L, "spd");
	p3d->spd_var = LuaDataHelper::GetIntField(L, "spd_var");
	p3d->angular_spd = TransDegree(LuaDataHelper::GetIntField(L, "angular_spd"));
	p3d->angular_spd_var = TransDegree(LuaDataHelper::GetIntField(L, "angular_spd_var"));

	p3d->dis_region = LuaDataHelper::GetIntField(L, "dis_region");
	p3d->dis_region_var = LuaDataHelper::GetIntField(L, "dis_region_var");
	p3d->dis_spd = LuaDataHelper::GetIntField(L, "dis_spd");
	p3d->dis_spd_var = LuaDataHelper::GetIntField(L, "dis_spd_var");

	p3d->gravity = LuaDataHelper::GetIntField(L, "gravity");

	p3d->linear_acc = LuaDataHelper::GetIntField(L, "linear_acc");
	p3d->linear_acc_var = LuaDataHelper::GetIntField(L, "linear_acc_var");

	p3d->fadeout_time = TransTime(LuaDataHelper::GetIntField(L, "fadeout_time"));

	p3d->bounce = LuaDataHelper::GetBoolField(L, "bounce");

	p3d->start_radius = LuaDataHelper::GetIntField(L, "start_radius");
	p3d->is_start_radius_3d = LuaDataHelper::GetBoolField(L, "is_start_radius_3d");

	p3d->orient_to_movement = LuaDataHelper::GetBoolField(L, "orient_to_movement");
}

float Particle3DFromLua::TransTime(int time)
{
	return time * 0.001f;
}

float Particle3DFromLua::TransDegree(int deg)
{
	return deg * d2d::TRANS_DEG_TO_RAD;
}

float Particle3DFromLua::TransFloatX100(int f)
{
	return f * 0.01f;
}

}