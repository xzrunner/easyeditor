#include "Particle2DFromLua.h"
#include "UnpackNodeFactory.h"

#include <epbin.h>
#include <particle2d.h>

namespace librespacker
{

void Particle2DFromLua::Unpack(lua_State* L, PackParticle2D* p2d)
{
	p2d->components.clear();

	if (epbin::LuaDataHelper::HasField(L, "export")) {
		p2d->export_name = epbin::LuaDataHelper::GetStringField(L, "export");
	}

	UnpackComponents(L, p2d);
	UnpackBody(L, p2d);
}

void Particle2DFromLua::UnpackComponents(lua_State* L, PackParticle2D* p2d)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	lua_getfield(L, -1, "component");
	int comp_sz = lua_rawlen(L, -1);
	p2d->components.reserve(comp_sz);
	for (int i = 1; i <= comp_sz; ++i)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, -2);
		assert(lua_istable(L, -1));

		p2d->components.push_back(PackParticle2D::Component());
		PackParticle2D::Component& comp = p2d->components[p2d->components.size() - 1];

		int id = epbin::LuaDataHelper::GetIntField(L, "id");
		comp.node = factory->Query(id);
		if (!comp.node) {
			factory->AddUnassigned(id, &comp.node);
		}

		comp.angle_start = TransDegree(epbin::LuaDataHelper::GetIntField(L, "angle_start"));
		comp.angle_end = TransDegree(epbin::LuaDataHelper::GetIntField(L, "angle_end"));

		comp.scale_start = TransFloatX100(epbin::LuaDataHelper::GetIntField(L, "scale_start"));
		comp.scale_end = TransFloatX100(epbin::LuaDataHelper::GetIntField(L, "scale_end"));

		comp.col_mul_start = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "col_mul_start");
		comp.col_mul_end = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "col_mul_end");
		comp.col_add_start = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "col_add_start");
		comp.col_add_end = (uint32_t)epbin::LuaDataHelper::GetDoubleField(L, "col_add_end");

		lua_pop(L, 1);
	}
	lua_pop(L, 1);	
}

void Particle2DFromLua::UnpackBody(lua_State* L, PackParticle2D* p2d)
{
	p2d->capacity = epbin::LuaDataHelper::GetIntField(L, "capacity");

	p2d->emission_time = TransTime(epbin::LuaDataHelper::GetIntField(L, "emission_time"));
	p2d->count = epbin::LuaDataHelper::GetIntField(L, "count");

	p2d->life = TransTime(epbin::LuaDataHelper::GetIntField(L, "life"));
	p2d->life_var = TransTime(epbin::LuaDataHelper::GetIntField(L, "life_var"));

	p2d->position.x = epbin::LuaDataHelper::GetIntField(L, "position_x");
	p2d->position.y = epbin::LuaDataHelper::GetIntField(L, "position_y");
	p2d->position_var.x = epbin::LuaDataHelper::GetIntField(L, "position_var_x");
	p2d->position_var.y = epbin::LuaDataHelper::GetIntField(L, "position_var_y");

	p2d->direction = TransDegree(epbin::LuaDataHelper::GetIntField(L, "direction"));
	p2d->direction_var = TransDegree(epbin::LuaDataHelper::GetIntField(L, "direction_var"));

	p2d->mode_type = epbin::LuaDataHelper::GetIntField(L, "mode_type");

	if (p2d->mode_type == P2D_MODE_GRAVITY) 
	{
		p2d->A.gravity.x = epbin::LuaDataHelper::GetIntField(L, "gravity_x");
		p2d->A.gravity.y = epbin::LuaDataHelper::GetIntField(L, "gravity_y");

		p2d->A.speed = epbin::LuaDataHelper::GetIntField(L, "speed");
		p2d->A.speed_var = epbin::LuaDataHelper::GetIntField(L, "speed_var");

		p2d->A.tangential_accel = epbin::LuaDataHelper::GetIntField(L, "tangential_accel");
		p2d->A.tangential_accel_var = epbin::LuaDataHelper::GetIntField(L, "tangential_accel_var");
		p2d->A.radial_accel = epbin::LuaDataHelper::GetIntField(L, "radial_accel");
		p2d->A.radial_accel_var = epbin::LuaDataHelper::GetIntField(L, "radial_accel_var");

		p2d->A.rotation_is_dir = epbin::LuaDataHelper::GetBoolField(L, "rotation_is_dir");
	}
	else if (p2d->mode_type == P2D_MODE_RADIUS)
	{
		p2d->B.start_radius = epbin::LuaDataHelper::GetIntField(L, "start_radius");
		p2d->B.start_radius_var = epbin::LuaDataHelper::GetIntField(L, "start_radius_var");
		p2d->B.end_radius = epbin::LuaDataHelper::GetIntField(L, "end_radius");
		p2d->B.end_radius_var = epbin::LuaDataHelper::GetIntField(L, "end_radius_var");

		p2d->B.direction_delta = epbin::LuaDataHelper::GetIntField(L, "direction_delta");
		p2d->B.direction_delta_var = epbin::LuaDataHelper::GetIntField(L, "direction_delta_var");
	}
	else if (p2d->mode_type == P2D_MODE_SPD_COS)
	{
		p2d->C.speed = epbin::LuaDataHelper::GetIntField(L, "speed");
		p2d->C.speed_var = epbin::LuaDataHelper::GetIntField(L, "speed_var");

		p2d->C.cos_amplitude = epbin::LuaDataHelper::GetIntField(L, "cos_amplitude");
		p2d->C.cos_amplitude_var = epbin::LuaDataHelper::GetIntField(L, "cos_amplitude_var");
		p2d->C.cos_frequency = epbin::LuaDataHelper::GetIntField(L, "cos_frequency");
		p2d->C.cos_frequency_var = epbin::LuaDataHelper::GetIntField(L, "cos_frequency_var");
	}
	else 
	{
		throw d2d::Exception("Particle2DFromLua::UnpackBody unknown mode type.");
	}
}

float Particle2DFromLua::TransTime(int time)
{
	return time * 0.001f;
}

float Particle2DFromLua::TransDegree(int deg)
{
	return deg * d2d::TRANS_DEG_TO_RAD;
}

float Particle2DFromLua::TransFloatX100(int f)
{
	return f * 0.01f;
}

}