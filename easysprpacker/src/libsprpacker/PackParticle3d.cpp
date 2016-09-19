#include "PackParticle3D.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <ee/Symbol.h>

#include <easyparticle3d.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeParticle3d.h>
#include <simp/simp_types.h>
#include <gum/trans_color.h>

namespace esprpacker
{

PackParticle3d::PackParticle3d(const eparticle3d::Symbol* sym)
{
	Init(sym);
}

void PackParticle3d::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"particle3d\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));
	if (!m_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + m_name + "\"");
	}

	{
		lua::TableAssign ta(gen, "components", true);
		for (int i = 0, n = m_components.size(); i < n; ++i) {
			m_components[i].PackToLuaString(gen);
		}
	}
	PackToLuaPS(gen);

	gen.detab();
	gen.line("},");
}

int PackParticle3d::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeParticle3d::Size();
	for (int i = 0, n = m_components.size(); i < n; ++i) {
		sz += m_components[i].SizeOfUnpackFromBin();
	}
	return sz;
}

int PackParticle3d::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);				// id
	sz += sizeof(uint8_t);				// type
	// components
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_components.size(); i < n; ++i) {
		sz += m_components[i].SizeOfPackToBin();
	}
	sz += sizeof(uint8_t);				// static_mode
	sz += sizeof(uint16_t) * 2;			// emission_time, count
	sz += sizeof(uint16_t) * 2;			// life
	sz += sizeof(uint16_t) * 4;			// dir
	sz += sizeof(uint16_t) * 2;			// radial spd
	sz += sizeof(uint16_t) * 2;			// tangential spd
	sz += sizeof(uint16_t) * 2;			// angular spd
	sz += sizeof(uint16_t) * 4;			// dis
	sz += sizeof(uint16_t);				// gravity
	sz += sizeof(uint16_t) * 2;			// acc
	sz += sizeof(uint16_t);				// fadeout_time
	sz += sizeof(uint8_t);				// ground
	sz += sizeof(uint16_t) * 2;			// start position
	sz += sizeof(uint8_t);				// orient_to_movement
	sz += sizeof(uint8_t);				// blend
	return sz;
}

void PackParticle3d::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_PARTICLE3D;
	pack(type, ptr);

	// components
	uint16_t n = m_components.size();
	pack(n, ptr);
	for (int i = 0; i < n; ++i) {
		m_components[i].PackToBin(ptr);
	}

	// body

	uint8_t static_mode = bool2int(m_static_mode);
	pack(static_mode, ptr);

	uint16_t emission_time = float100x2int(m_emission_time);
	pack(emission_time, ptr);
	uint16_t count = m_count;
	pack(count, ptr);

	uint16_t life = float100x2int(m_life);
	pack(life, ptr);
	uint16_t life_var = float100x2int(m_life_var);
	pack(life_var, ptr);

	int16_t hori = radian2int(m_hori);
	pack(hori, ptr);
	uint16_t hori_var = radian2int(m_hori_var);
	pack(hori_var, ptr);
	int16_t vert = radian2int(m_vert);
	pack(vert, ptr);
	uint16_t vert_var = radian2int(m_vert_var);
	pack(vert_var, ptr);

	uint16_t radial_spd = float2int(m_radial_spd);
	pack(radial_spd, ptr);
	uint16_t radial_spd_var = float2int(m_radial_spd_var);
	pack(radial_spd_var, ptr);

	int16_t tangential_spd = float2int(m_tangential_spd);
	pack(tangential_spd, ptr);
	uint16_t tangential_spd_var = float2int(m_tangential_spd_var);
	pack(tangential_spd_var, ptr);

	int16_t angular_spd = radian2int(m_angular_spd);
	pack(angular_spd, ptr);
	uint16_t angular_spd_var = radian2int(m_angular_spd_var);
	pack(angular_spd_var, ptr);

	uint16_t dis_region = float2int(m_dis_region);
	pack(dis_region, ptr);
	uint16_t dis_region_var = float2int(m_dis_region_var);
	pack(dis_region_var, ptr);
	uint16_t dis_spd = float2int(m_dis_spd);
	pack(dis_spd, ptr);
	uint16_t dis_spd_var = float2int(m_dis_spd_var);
	pack(dis_spd_var, ptr);

	int16_t gravity = float2int(m_gravity);
	pack(gravity, ptr);

	int16_t linear_acc = float2int(m_linear_acc);
	pack(linear_acc, ptr);
	uint16_t linear_acc_var = float2int(m_linear_acc_var);
	pack(linear_acc_var, ptr);

	uint16_t fadeout_time = float100x2int(m_fadeout_time);
	pack(fadeout_time, ptr);

	uint8_t ground = m_ground;
	pack(ground, ptr);

	uint16_t start_radius = float2int(m_start_radius);
	pack(start_radius, ptr);
	int16_t start_height = float2int(m_start_height);
	pack(start_height, ptr);

	uint8_t orient_to_movement = bool2int(m_orient_to_movement);
	pack(orient_to_movement, ptr);

	uint8_t blend = m_blend;
	pack(blend, ptr);
}

void PackParticle3d::Init(const eparticle3d::Symbol* sym)
{
	const p3d_emitter_cfg* cfg = sym->GetEmitterCfg();

	m_blend				= cfg->blend;

	m_static_mode		= cfg->static_mode;

	m_emission_time		= cfg->emission_time;
	m_count				= cfg->count;

	m_life				= cfg->life;
	m_life_var			= cfg->life_var;

	m_hori				= cfg->hori;
	m_hori_var			= cfg->hori_var;
	m_vert				= cfg->vert;
	m_vert_var			= cfg->vert_var;

	m_radial_spd		= cfg->radial_spd;
	m_radial_spd_var	= cfg->radial_spd_var;
	m_tangential_spd	= cfg->tangential_spd;
	m_tangential_spd_var= cfg->tangential_spd_var;
	m_angular_spd		= cfg->angular_spd;
	m_angular_spd_var	= cfg->angular_spd_var;

	m_dis_region		= cfg->dis_region;
	m_dis_region_var	= cfg->dis_region_var;
	m_dis_spd			= cfg->dis_spd;
	m_dis_spd_var		= cfg->dis_spd_var;

	m_gravity			= cfg->gravity;

	m_linear_acc		= cfg->linear_acc;
	m_linear_acc_var	= cfg->linear_acc_var;

	m_fadeout_time		= cfg->fadeout_time;

	m_ground			= cfg->ground;

	m_start_radius		= cfg->start_radius;
	m_start_height		= cfg->start_height;

	m_orient_to_movement= cfg->orient_to_movement;

	m_components.reserve(cfg->sym_count);
	for (int i = 0; i < cfg->sym_count; ++i) {
		m_components.push_back(Component(cfg->syms[i]));
	}
}

void PackParticle3d::PackToLuaPS(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 3, 
		lua::assign("static_mode", m_static_mode), 
		lua::assign("emission_time", m_emission_time), 
		lua::assign("count", m_count));

	lua::connect(gen, 2, 
		lua::assign("life", m_life), 
		lua::assign("life_var", m_life_var));

	lua::connect(gen, 4, 
		lua::assign("hori", m_hori), 
		lua::assign("hori_var", m_hori_var),
		lua::assign("vert", m_vert), 
		lua::assign("vert_var", m_vert_var));

	lua::connect(gen, 6, 
		lua::assign("radial_spd", m_radial_spd), 
		lua::assign("radial_spd_var", m_radial_spd_var),
		lua::assign("tangential_spd", m_tangential_spd), 
		lua::assign("tangential_spd_var", m_tangential_spd_var),
		lua::assign("angular_spd", m_angular_spd), 
		lua::assign("angular_spd_var", m_angular_spd_var));

	lua::connect(gen, 4, 
		lua::assign("dis_region", m_dis_region), 
		lua::assign("dis_region_var", m_dis_region_var),
		lua::assign("dis_spd", m_dis_spd), 
		lua::assign("dis_spd_var", m_dis_spd_var));

	lua::connect(gen, 1, 
		lua::assign("gravity", m_gravity));

	lua::connect(gen, 2, 
		lua::assign("linear_acc", m_linear_acc), 
		lua::assign("linear_acc_var", m_linear_acc_var));

	lua::connect(gen, 1, 
		lua::assign("fadeout_time", m_fadeout_time));

	lua::connect(gen, 1, 
		lua::assign("ground", m_ground));

	lua::connect(gen, 2, 
		lua::assign("start_radius", m_start_radius), 
		lua::assign("start_height", m_start_height));

	lua::connect(gen, 1, 
		lua::assign("orient_to_movement", m_orient_to_movement));

	lua::connect(gen, 1, 
		lua::assign("blend", m_blend));
}

/************************************************************************/
/* class PackParticle3d::Component                                      */
/************************************************************************/

PackParticle3d::Component::
Component(const p3d_symbol& sym)
{
	m_count				= sym.count;

	m_scale_start		= sym.scale_start;
	m_scale_end			= sym.scale_end;

	m_angle				= sym.angle;
	m_angle_var			= sym.angle_var;

	m_mul_col_begin		= gum::color2int(sym.mul_col_begin.rgba, gum::PT_RGBA);
	m_mul_col_end		= gum::color2int(sym.mul_col_end.rgba, gum::PT_RGBA);
	m_add_col_begin		= gum::color2int(sym.add_col_begin.rgba, gum::PT_RGBA);
	m_add_col_end		= gum::color2int(sym.add_col_end.rgba, gum::PT_RGBA);

	s2::Symbol* s2_sym = static_cast<s2::Symbol*>(sym.ud);
	m_node = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(s2_sym));
}

PackParticle3d::Component::
~Component()
{
	m_node->RemoveReference();
}

void PackParticle3d::Component::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign ta(gen, "", true);

	lua::connect(gen, 1, 
		lua::assign("id", m_node->GetID()));

	lua::connect(gen, 1, 
		lua::assign("count", m_count));

	lua::connect(gen, 2, 
		lua::assign("scale_start", m_scale_start), 
		lua::assign("scale_end", m_scale_end));

	lua::connect(gen, 2, 
		lua::assign("angle", m_angle), 
		lua::assign("angle_var", m_angle_var));

	lua::connect(gen, 4, 
		lua::assign("mul_col_begin", m_mul_col_begin), 
		lua::assign("mul_col_end", m_mul_col_end),
		lua::assign("add_col_begin", m_add_col_begin), 
		lua::assign("add_col_end", m_add_col_end));
}

int PackParticle3d::Component::
SizeOfUnpackFromBin() const
{
	return simp::NodeParticle3d::ComponentSize();
}

int PackParticle3d::Component::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint16_t);			// count
	sz += sizeof(uint16_t) * 2;		// scale
	sz += sizeof(uint16_t) * 2;		// angle
	sz += sizeof(uint32_t) * 4;		// color
	return sz;
}

void PackParticle3d::Component::
PackToBin(uint8_t** ptr) const
{
	uint32_t id = m_node->GetID();
	pack(id, ptr);

	uint16_t count = m_count;
	pack(count, ptr);

	uint16_t scale_start = float100x2int(m_scale_start);
	pack(scale_start, ptr);
	uint16_t scale_end = float100x2int(m_scale_end);
	pack(scale_end, ptr);

	int16_t angle = float2int(m_angle);
	pack(angle, ptr);
	uint16_t angle_var = float2int(m_angle_var);
	pack(angle_var, ptr);

	uint32_t mul_col_begin = m_mul_col_begin;
	pack(mul_col_begin, ptr);
	uint32_t mul_col_end = m_mul_col_end;
	pack(mul_col_end, ptr);
	uint32_t add_col_begin = m_add_col_begin;
	pack(add_col_begin, ptr);
	uint32_t add_col_end = m_add_col_end;
	pack(add_col_end, ptr);
}

}