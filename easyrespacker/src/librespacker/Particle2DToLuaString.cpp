#include "Particle2DToLuaString.h"

#include <ee/Exception.h>

#include <easybuilder.h>
#include <ps_2d.h>

namespace lua = ebuilder::lua;

namespace erespacker
{

void Particle2DToLuaString::Pack(const PackParticle2D* p2d, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + p2d->GetFilepath());

	lua::assign_with_end(gen, "type", "\"particle2d\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(p2d->GetSprID()));
	if (!p2d->export_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + p2d->export_name + "\"");
	}

	{
		lua::TableAssign ta(gen, "component", true);
		for (int i = 0, n = p2d->components.size(); i < n; ++i) {
			PackComponent(p2d->components[i], gen);
		}
	}
	PackPS(p2d, gen);

	gen.detab();
	gen.line("},");
}

void Particle2DToLuaString::PackPS(const PackParticle2D* p2d, ebuilder::CodeGenerator& gen)
{
	lua::connect(gen, 2, 
		lua::assign("emission_time", TransTime(p2d->emission_time)), 
		lua::assign("count", p2d->count));

	lua::connect(gen, 2, 
		lua::assign("life", TransTime(p2d->life)), 
		lua::assign("life_var", TransTime(p2d->life_var)));

	lua::connect(gen, 4, 
		lua::assign("position_x", TransFloat(p2d->position.x)), 
		lua::assign("position_y", TransFloat(p2d->position.y)),
		lua::assign("position_var_x", TransFloat(p2d->position_var.x)), 
		lua::assign("position_var_y", TransFloat(p2d->position_var.y)));

	lua::connect(gen, 2, 
		lua::assign("direction", TransRadian(p2d->direction)), 
		lua::assign("direction_var", TransRadian(p2d->direction_var)));

	lua::connect(gen, 1, 
		lua::assign("mode_type", p2d->mode_type));

	if (p2d->mode_type == P2D_MODE_GRAVITY) 
	{
		lua::connect(gen, 2, 
			lua::assign("gravity_x", TransFloat(p2d->A.gravity.x)), 
			lua::assign("gravity_y", TransFloat(p2d->A.gravity.y)));

		lua::connect(gen, 2, 
			lua::assign("speed", TransFloat(p2d->A.speed)), 
			lua::assign("speed_var", TransFloat(p2d->A.speed_var)));
		
		lua::connect(gen, 2, 
			lua::assign("tangential_accel", TransFloat(p2d->A.tangential_accel)), 
			lua::assign("tangential_accel_var", TransFloat(p2d->A.tangential_accel_var)));
		lua::connect(gen, 2, 
			lua::assign("radial_accel", TransFloat(p2d->A.radial_accel)), 
			lua::assign("radial_accel_var", TransFloat(p2d->A.radial_accel_var)));

		lua::connect(gen, 1, 
			lua::assign("rotation_is_dir", TransBool(p2d->A.rotation_is_dir)));
	}
	else if (p2d->mode_type == P2D_MODE_RADIUS)
	{
		lua::connect(gen, 2, 
			lua::assign("start_radius", TransFloat(p2d->B.start_radius)), 
			lua::assign("start_radius_var", TransFloat(p2d->B.start_radius_var)));
		lua::connect(gen, 2, 
			lua::assign("end_radius", TransFloat(p2d->B.end_radius)), 
			lua::assign("end_radius_var", TransFloat(p2d->B.end_radius_var)));

		lua::connect(gen, 2, 
			lua::assign("direction_delta", TransFloat(p2d->B.direction_delta)), 
			lua::assign("direction_delta_var", TransFloat(p2d->B.direction_delta_var)));
	}
	else if (p2d->mode_type == P2D_MODE_SPD_COS)
	{
		lua::connect(gen, 2, 
			lua::assign("speed", TransFloat(p2d->C.speed)), 
			lua::assign("speed_var", TransFloat(p2d->C.speed_var)));

		lua::connect(gen, 2, 
			lua::assign("cos_amplitude", TransFloat(p2d->C.cos_amplitude)), 
			lua::assign("cos_amplitude_var", TransFloat(p2d->C.cos_amplitude_var)));
		lua::connect(gen, 2, 
			lua::assign("cos_frequency", TransFloat(p2d->C.cos_frequency)), 
			lua::assign("cos_frequency_var", TransFloat(p2d->C.cos_frequency_var)));
	}
	else
	{
		throw ee::Exception("Particle2DToLuaString::PackPS unknown mode type.");
	}
}

void Particle2DToLuaString::PackComponent(const PackParticle2D::Component& comp, 
										  ebuilder::CodeGenerator& gen)
{
 	lua::TableAssign ta(gen, "", true);
 
 	lua::connect(gen, 1, 
 		lua::assign("id", comp.node->GetSprID()));

	lua::connect(gen, 2, 
		lua::assign("angle_start", TransRadian(comp.angle_start)), 
		lua::assign("angle_end", TransRadian(comp.angle_end)));

 	lua::connect(gen, 2, 
 		lua::assign("scale_start", TransFloatX100(comp.scale_start)), 
 		lua::assign("scale_end", TransFloatX100(comp.scale_end)));
 
 	lua::connect(gen, 4, 
 		lua::assign("col_mul_start", comp.col_mul_start), 
 		lua::assign("col_mul_end", comp.col_mul_end),
 		lua::assign("col_add_start", comp.col_add_start), 
 		lua::assign("col_add_end", comp.col_add_end));
}

}