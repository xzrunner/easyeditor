#include "Particle3DToLuaString.h"

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace libcoco
{

void Particle3DToLuaString::Pack(const PackParticle3D* p3d, ebuilder::CodeGenerator& gen)
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + p3d->GetFilepath());

	lua::assign_with_end(gen, "type", "\"particle3d\"");
	lua::assign_with_end(gen, "id", d2d::StringTools::ToString(p3d->GetID()));

	PackPS(p3d, gen);
	for (int i = 0, n = p3d->components.size(); i < n; ++i) {
		PackComponent(p3d->components[i], gen);
	}

	gen.detab();
	gen.line("},");
}

void Particle3DToLuaString::PackPS(const PackParticle3D* p3d, ebuilder::CodeGenerator& gen)
{
	std::ostringstream ss;

	ss << "emission_time = \"" << TransTime(p3d->emission_time) << "\", ";
	ss << "count = \"" << p3d->count << "\", ";

	ss << "life = \"" << TransTime(p3d->life) << "\", ";
	ss << "life_var = \"" << TransTime(p3d->life_var) << "\", ";

	ss << "hori = \"" << TransRadian(p3d->hori) << "\", ";
	ss << "hori_var = \"" << TransRadian(p3d->hori_var) << "\", ";
	ss << "vert = \"" << TransRadian(p3d->vert) << "\", ";
	ss << "vert_var = \"" << TransRadian(p3d->vert_var) << "\", ";

	ss << "spd = \"" << TransSpeed(p3d->spd) << "\", ";
	ss << "spd_var = \"" << TransSpeed(p3d->spd_var) << "\", ";
	ss << "angular_spd = \"" << TransFloat(p3d->angular_spd) << "\", ";
	ss << "angular_spd_var = \"" << TransFloat(p3d->angular_spd_var) << "\", ";

	ss << "dis_region = \"" << TransFloat(p3d->dis_region) << "\", ";
	ss << "dis_region_var = \"" << TransFloat(p3d->dis_region_var) << "\", ";
	ss << "dis_spd = \"" << TransFloat(p3d->dis_spd) << "\", ";
	ss << "dis_spd_var = \"" << TransFloat(p3d->dis_spd_var) << "\", ";

	ss << "gravity = \"" << TransGravity(p3d->gravity) << "\", ";

	ss << "linear_acc = \"" << TransFloat(p3d->linear_acc) << "\", ";
	ss << "linear_acc_var = \"" << TransFloat(p3d->linear_acc_var) << "\", ";

	ss << "fadeout_time = \"" << TransTime(p3d->fadeout_time) << "\", ";

	ss << "bounce = \"" << TransBool(p3d->bounce) << "\", ";

	ss << "start_radius = \"" << TransFloat(p3d->start_radius) << "\", ";
	ss << "is_start_radius_3d = \"" << TransBool(p3d->is_start_radius_3d) << "\", ";

	ss << "orient_to_movement = \"" << TransBool(p3d->orient_to_movement) << "\", ";

	gen.line(ss.str());
}

void Particle3DToLuaString::PackComponent(const PackParticle3D::Component& comp, 
										  ebuilder::CodeGenerator& gen)
{
	std::ostringstream ss;

	ss << "scale_start = \"" << TransFloat(comp.scale_start) << "\", ";
	ss << "scale_end = \"" << TransFloat(comp.scale_end) << "\", ";

	ss << "angle = \"" << TransFloat(comp.angle) << "\", ";
	ss << "angle_var = \"" << TransFloat(comp.angle_var) << "\", ";

	ss << "col_mul = \"" << comp.col_mul << "\", ";
	ss << "col_add = \"" << comp.col_add << "\", ";
	ss << "alpha_start = \"" << TransFloat(comp.alpha_start) << "\", ";
	ss << "alpha_end = \"" << TransFloat(comp.alpha_end) << "\", ";

	ss << "id = \"" << comp.node->GetID() << "\", ";	

	gen.line(ss.str());
}

int Particle3DToLuaString::TransTime(float time)
{
	return floor(time * 1000 + 0.5f);
}

int Particle3DToLuaString::TransRadian(float r)
{
	return floor(r * d2d::TRANS_RAD_TO_DEG + 0.5f);
}

int Particle3DToLuaString::TransSpeed(float spd)
{
	return floor(spd * 0.25f + 0.5f);
}

int Particle3DToLuaString::TransFloat(float f)
{
	return floor(f + 0.5f);
}

int Particle3DToLuaString::TransGravity(float g)
{
	return floor(g * 0.3f + 0.5f);	
}

std::string Particle3DToLuaString::TransBool(bool b)
{
	return b ? "true" : "false";
}

}