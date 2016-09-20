#include "PackParticle2D.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <ee/Symbol.h>

#include <easyparticle2d.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeParticle2d.h>
#include <simp/simp_types.h>
#include <gum/trans_color.h>

namespace esprpacker
{

PackParticle2d::PackParticle2d(const eparticle2d::Symbol* sym)
{
	Init(sym);
}

void PackParticle2d::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"particle2d\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));
	if (!m_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + m_name + "\"");
	}

	{
		lua::TableAssign ta(gen, "component", true);
		for (int i = 0, n = m_components.size(); i < n; ++i) {
			m_components[i].PackToLuaString(gen);
		}
	}
	PackToLuaPS(gen);

	gen.detab();
	gen.line("},");
}

int PackParticle2d::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeParticle2d::Size();
	for (int i = 0, n = m_components.size(); i < n; ++i) {
		sz += m_components[i].SizeOfUnpackFromBin();
	}
	return sz;
}

int PackParticle2d::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);						// id
	sz += sizeof(uint8_t);						// type
	// components
	sz += sizeof(uint16_t);
	for (int i = 0, n = m_components.size(); i < n; ++i) {
		sz += m_components[i].SizeOfPackToBin();
	}
	sz += sizeof(uint16_t) * 2;					// emission_time, count
	sz += sizeof(uint16_t) * 2;					// life
	sz += sizeof(uint16_t) * 4;					// position
	sz += sizeof(uint16_t) * 2;					// direction
	sz += sizeof(uint8_t);						// mode type
	if (m_mode_type == P2D_MODE_GRAVITY) 
	{
		sz += sizeof(uint16_t) * 2;					// gravity
		sz += sizeof(uint16_t) * 2;					// speed
		sz += sizeof(uint16_t) * 2;					// tangential_accel
		sz += sizeof(uint16_t) * 2;					// radial_accel
		sz += sizeof(uint8_t);						// rotation_is_dir
	}
	else if (m_mode_type == P2D_MODE_RADIUS)
	{
		sz += sizeof(uint16_t) * 4;					// radius
		sz += sizeof(uint16_t) * 2;					// direction_delta
	}
	else if (m_mode_type == P2D_MODE_SPD_COS)
	{
		sz += sizeof(uint16_t) * 2;					// speed
		sz += sizeof(uint16_t) * 4;					// cos
	}
	else
	{
		throw ee::Exception("Particle2DToBin::Size unknown mode type.");
	}

	return sz;
}

void PackParticle2d::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_PARTICLE2D;
	pack(type, ptr);

	// components
	uint16_t sz = m_components.size();
	pack(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		m_components[i].PackToBin(ptr);
	}

	// body

	uint16_t emission_time = float100x2int(m_emission_time);
	pack(emission_time, ptr);
	uint16_t count = m_count;
	pack(count, ptr);

	uint16_t life = float100x2int(m_life);
	pack(life, ptr);
	uint16_t life_var = float100x2int(m_life_var);
	pack(life_var, ptr);

	uint16_t position_x = float2int(m_position.x);
	pack(position_x, ptr);
	uint16_t position_y = float2int(m_position.y);
	pack(position_y, ptr);
	uint16_t position_var_x = float2int(m_position_var.x);
	pack(position_var_x, ptr);
	uint16_t position_var_y = float2int(m_position_var.y);
	pack(position_var_y, ptr);

	uint16_t direction = radian2int(m_direction);
	pack(direction, ptr);
	uint16_t direction_var = radian2int(m_direction_var);
	pack(direction_var, ptr);

	uint8_t mode_type = m_mode_type;
	pack(mode_type, ptr);

	if (m_mode_type == P2D_MODE_GRAVITY) 
	{
		uint16_t gravity_x = float2int(A.gravity.x);
		pack(gravity_x, ptr);
		uint16_t gravity_y = float2int(A.gravity.y);
		pack(gravity_y, ptr);

		uint16_t speed = float2int(A.speed);
		pack(speed, ptr);
		uint16_t speed_var = float2int(A.speed_var);
		pack(speed_var, ptr);

		uint16_t tangential_accel = float2int(A.tangential_accel);
		pack(tangential_accel, ptr);
		uint16_t tangential_accel_var = float2int(A.tangential_accel_var);
		pack(tangential_accel_var, ptr);

		uint16_t radial_accel = float2int(A.radial_accel);
		pack(radial_accel, ptr);
		uint16_t radial_accel_var = float2int(A.radial_accel_var);
		pack(radial_accel_var, ptr);

		uint8_t rotation_is_dir = bool2int(A.rotation_is_dir);
		pack(rotation_is_dir, ptr);
	}
	else if (m_mode_type == P2D_MODE_RADIUS)
	{
		uint16_t start_radius = float2int(B.start_radius);
		pack(start_radius, ptr);
		uint16_t start_radius_var = float2int(B.start_radius_var);
		pack(start_radius_var, ptr);

		uint16_t end_radius = float2int(B.end_radius);
		pack(end_radius, ptr);
		uint16_t end_radius_var = float2int(B.end_radius_var);
		pack(end_radius_var, ptr);

		uint16_t direction_delta = float2int(B.direction_delta);
		pack(direction_delta, ptr);
		uint16_t direction_delta_var = float2int(B.direction_delta_var);
		pack(direction_delta_var, ptr);
	}
	else if (m_mode_type == P2D_MODE_SPD_COS)
	{
		uint16_t speed = float2int(C.speed);
		pack(speed, ptr);
		uint16_t speed_var = float2int(C.speed_var);
		pack(speed_var, ptr);

		uint16_t cos_amplitude = float2int(C.cos_amplitude);
		pack(cos_amplitude, ptr);
		uint16_t cos_amplitude_var = float2int(C.cos_amplitude_var);
		pack(cos_amplitude_var, ptr);

		uint16_t cos_frequency = float2int(C.cos_frequency);
		pack(cos_frequency, ptr);
		uint16_t cos_frequency_var = float2int(C.cos_frequency_var);
		pack(cos_frequency_var, ptr);
	}
	else
	{
		throw ee::Exception("Particle2DToBin::Pack unknown mode type.");
	}
}

void PackParticle2d::Init(const eparticle2d::Symbol* sym)
{
	const p2d_emitter_cfg* cfg = sym->GetEmitterCfg();

	m_emission_time		= cfg->emission_time;
	m_count				= cfg->count;

	m_life				= cfg->life;
	m_life_var			= cfg->life_var;

	m_position.x		= cfg->position.x;
	m_position.y		= cfg->position.y;
	m_position_var.x	= cfg->position_var.x;
	m_position_var.y	= cfg->position_var.y;

	m_direction			= cfg->direction;
	m_direction_var		= cfg->direction_var;

	m_mode_type			= cfg->mode_type;
	if (m_mode_type == P2D_MODE_GRAVITY)
	{
		A.gravity.x				= cfg->mode.A.gravity.x;
		A.gravity.y				= cfg->mode.A.gravity.y;

		A.speed					= cfg->mode.A.speed;
		A.speed_var				= cfg->mode.A.speed_var;

		A.tangential_accel		= cfg->mode.A.tangential_accel;
		A.tangential_accel_var	= cfg->mode.A.tangential_accel_var;
		A.radial_accel			= cfg->mode.A.radial_accel;
		A.radial_accel_var		= cfg->mode.A.radial_accel_var;

		A.rotation_is_dir		= cfg->mode.A.rotation_is_dir;
	}
	else if (m_mode_type == P2D_MODE_RADIUS)
	{
		B.start_radius			= cfg->mode.B.start_radius;
		B.start_radius_var		= cfg->mode.B.start_radius_var;
		B.end_radius			= cfg->mode.B.end_radius;
		B.end_radius_var		= cfg->mode.B.end_radius_var;

		B.direction_delta		= cfg->mode.B.direction_delta;
		B.direction_delta_var	= cfg->mode.B.direction_delta_var;
	}
	else if (m_mode_type == P2D_MODE_SPD_COS)
	{
		C.speed					= cfg->mode.C.speed;
		C.speed_var				= cfg->mode.C.speed_var;

		C.cos_amplitude			= cfg->mode.C.cos_amplitude;
		C.cos_amplitude_var		= cfg->mode.C.cos_amplitude_var;
		C.cos_frequency			= cfg->mode.C.cos_frequency;
		C.cos_frequency_var		= cfg->mode.C.cos_frequency_var;
	}
	else
	{
		throw ee::Exception("Particle2DBuilder::Load unknown mode type, filepath: %s", sym->GetFilepath().c_str());
	}


	m_components.reserve(cfg->sym_count);
	for (int i = 0; i < cfg->sym_count; ++i) {
		m_components.push_back(Component(cfg->syms[i]));
	}
}

void PackParticle2d::PackToLuaPS(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 2, 
		lua::assign("emission_time", m_emission_time), 
		lua::assign("count", m_count));

	lua::connect(gen, 2, 
		lua::assign("life", m_life), 
		lua::assign("life_var", m_life_var));

	lua::connect(gen, 4, 
		lua::assign("position_x", m_position.x), 
		lua::assign("position_y", m_position.y),
		lua::assign("position_var_x", m_position_var.x), 
		lua::assign("position_var_y", m_position_var.y));

	lua::connect(gen, 2, 
		lua::assign("direction", m_direction), 
		lua::assign("direction_var", m_direction_var));

	lua::connect(gen, 1, 
		lua::assign("mode_type", m_mode_type));

	if (m_mode_type == P2D_MODE_GRAVITY) 
	{
		lua::connect(gen, 2, 
			lua::assign("gravity_x", A.gravity.x), 
			lua::assign("gravity_y", A.gravity.y));

		lua::connect(gen, 2, 
			lua::assign("speed", A.speed), 
			lua::assign("speed_var", A.speed_var));

		lua::connect(gen, 2, 
			lua::assign("tangential_accel", A.tangential_accel), 
			lua::assign("tangential_accel_var", A.tangential_accel_var));
		lua::connect(gen, 2, 
			lua::assign("radial_accel", A.radial_accel), 
			lua::assign("radial_accel_var", A.radial_accel_var));

		lua::connect(gen, 1, 
			lua::assign("rotation_is_dir", A.rotation_is_dir));
	}
	else if (m_mode_type == P2D_MODE_RADIUS)
	{
		lua::connect(gen, 2, 
			lua::assign("start_radius", B.start_radius), 
			lua::assign("start_radius_var", B.start_radius_var));
		lua::connect(gen, 2, 
			lua::assign("end_radius", B.end_radius), 
			lua::assign("end_radius_var", B.end_radius_var));

		lua::connect(gen, 2, 
			lua::assign("direction_delta", B.direction_delta), 
			lua::assign("direction_delta_var", B.direction_delta_var));
	}
	else if (m_mode_type == P2D_MODE_SPD_COS)
	{
		lua::connect(gen, 2, 
			lua::assign("speed", C.speed), 
			lua::assign("speed_var", C.speed_var));

		lua::connect(gen, 2, 
			lua::assign("cos_amplitude", C.cos_amplitude), 
			lua::assign("cos_amplitude_var", C.cos_amplitude_var));
		lua::connect(gen, 2, 
			lua::assign("cos_frequency", C.cos_frequency), 
			lua::assign("cos_frequency_var", C.cos_frequency_var));
	}
	else
	{
		throw ee::Exception("Particle2DToLuaString::PackPS unknown mode type.");
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

PackParticle2d::Component::
Component(const p2d_symbol& sym)
{
	m_angle_start		= sym.angle_start;
	m_angle_end			= sym.angle_end;

	m_scale_start		= sym.scale_start;
	m_scale_end			= sym.scale_end;

	m_mul_col_begin		= gum::color2int(sym.mul_col_begin.rgba, gum::RGBA);
	m_mul_col_end		= gum::color2int(sym.mul_col_end.rgba, gum::RGBA);
	m_add_col_begin		= gum::color2int(sym.add_col_begin.rgba, gum::RGBA);
	m_add_col_end		= gum::color2int(sym.add_col_end.rgba, gum::RGBA);

	ee::Symbol* ee_sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(sym.ud));
	m_node = PackNodeFactory::Instance()->Create(ee_sym);
}

PackParticle2d::Component::
~Component()
{
	m_node->RemoveReference();
}

void PackParticle2d::Component::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign ta(gen, "", true);

	lua::connect(gen, 1, 
		lua::assign("id", m_node->GetID()));

	lua::connect(gen, 2, 
		lua::assign("angle_start", m_angle_start), 
		lua::assign("angle_end", m_angle_end));

	lua::connect(gen, 2, 
		lua::assign("scale_start", m_scale_start), 
		lua::assign("scale_end", m_scale_end));

	lua::connect(gen, 4, 
		lua::assign("mul_col_begin", m_mul_col_begin), 
		lua::assign("mul_col_end", m_mul_col_end),
		lua::assign("add_col_begin", m_add_col_begin), 
		lua::assign("add_col_end", m_add_col_end));
}

int PackParticle2d::Component::
SizeOfUnpackFromBin() const
{
	return simp::NodeParticle2d::ComponentSize();
}

int PackParticle2d::Component::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint16_t) * 2;		// angle
	sz += sizeof(uint16_t) * 2;		// scale
	sz += sizeof(uint32_t) * 4;		// color
	return sz;
}

void PackParticle2d::Component::
PackToBin(uint8_t** ptr) const
{
	uint32_t id = m_node->GetID();
	pack(id, ptr);

	uint16_t angle_start = float100x2int(m_angle_start);
	pack(angle_start, ptr);
	uint16_t angle_end = float100x2int(m_angle_end);
	pack(angle_end, ptr);

	uint16_t scale_start = float100x2int(m_scale_start);
	pack(scale_start, ptr);
	uint16_t scale_end = float100x2int(m_scale_end);
	pack(scale_end, ptr);

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