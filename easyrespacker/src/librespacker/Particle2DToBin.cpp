#include "Particle2DToBin.h"
#include "pack_unpack.h"

#include <ps_2d.h>
#include <spritepack.h>

namespace erespacker
{

int Particle2DToBin::Size(const PackParticle2D* p2d)
{
	int sz = 0;
	sz += sizeof(uint16_t);						// id
	sz += sizeof(uint8_t);						// type
	sz += sizeof(uint16_t);						// component sz
	sz += ComponentSize() * p2d->components.size();
	sz += sizeof(uint16_t) * 2;					// emission_time, count
	sz += sizeof(uint16_t) * 2;					// life
	sz += sizeof(uint16_t) * 4;					// position
	sz += sizeof(uint16_t) * 2;					// direction
	sz += sizeof(uint8_t);						// mode type
	if (p2d->mode_type == P2D_MODE_GRAVITY) 
	{
		sz += sizeof(uint16_t) * 2;					// gravity
		sz += sizeof(uint16_t) * 2;					// speed
		sz += sizeof(uint16_t) * 2;					// tangential_accel
		sz += sizeof(uint16_t) * 2;					// radial_accel
		sz += sizeof(uint8_t);						// rotation_is_dir
	}
	else if (p2d->mode_type == P2D_MODE_RADIUS)
	{
		sz += sizeof(uint16_t) * 4;					// radius
		sz += sizeof(uint16_t) * 2;					// direction_delta
	}
	else if (p2d->mode_type == P2D_MODE_SPD_COS)
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

void Particle2DToBin::Pack(const PackParticle2D* p2d, uint8_t** ptr)
{
	uint16_t id = p2d->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_PARTICLE2D;
	pack(type, ptr);

	// components
	uint16_t sz = p2d->components.size();
	pack(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		PackComponent(p2d->components[i], ptr);
	}

	// body

	uint16_t emission_time = TransTime(p2d->emission_time);
	pack(emission_time, ptr);
	uint16_t count = p2d->count;
	pack(count, ptr);

	uint16_t life = TransTime(p2d->life);
	pack(life, ptr);
	uint16_t life_var = TransTime(p2d->life_var);
	pack(life_var, ptr);

	uint16_t position_x = TransFloat(p2d->position.x);
	pack(position_x, ptr);
	uint16_t position_y = TransFloat(p2d->position.y);
	pack(position_y, ptr);
	uint16_t position_var_x = TransFloat(p2d->position_var.x);
	pack(position_var_x, ptr);
	uint16_t position_var_y = TransFloat(p2d->position_var.y);
	pack(position_var_y, ptr);

	uint16_t direction = TransRadian(p2d->direction);
	pack(direction, ptr);
	uint16_t direction_var = TransRadian(p2d->direction_var);
	pack(direction_var, ptr);

	uint8_t mode_type = p2d->mode_type;
	pack(mode_type, ptr);

	if (p2d->mode_type == P2D_MODE_GRAVITY) 
	{
		uint16_t gravity_x = TransFloat(p2d->A.gravity.x);
		pack(gravity_x, ptr);
		uint16_t gravity_y = TransFloat(p2d->A.gravity.y);
		pack(gravity_y, ptr);

		uint16_t speed = TransFloat(p2d->A.speed);
		pack(speed, ptr);
		uint16_t speed_var = TransFloat(p2d->A.speed_var);
		pack(speed_var, ptr);
		
		uint16_t tangential_accel = TransFloat(p2d->A.tangential_accel);
		pack(tangential_accel, ptr);
		uint16_t tangential_accel_var = TransFloat(p2d->A.tangential_accel_var);
		pack(tangential_accel_var, ptr);

		uint16_t radial_accel = TransFloat(p2d->A.radial_accel);
		pack(radial_accel, ptr);
		uint16_t radial_accel_var = TransFloat(p2d->A.radial_accel_var);
		pack(radial_accel_var, ptr);

		uint8_t rotation_is_dir = TransBool(p2d->A.rotation_is_dir);
		pack(rotation_is_dir, ptr);
	}
	else if (p2d->mode_type == P2D_MODE_RADIUS)
	{
		uint16_t start_radius = TransFloat(p2d->B.start_radius);
		pack(start_radius, ptr);
		uint16_t start_radius_var = TransFloat(p2d->B.start_radius_var);
		pack(start_radius_var, ptr);

		uint16_t end_radius = TransFloat(p2d->B.end_radius);
		pack(end_radius, ptr);
		uint16_t end_radius_var = TransFloat(p2d->B.end_radius_var);
		pack(end_radius_var, ptr);

		uint16_t direction_delta = TransFloat(p2d->B.direction_delta);
		pack(direction_delta, ptr);
		uint16_t direction_delta_var = TransFloat(p2d->B.direction_delta_var);
		pack(direction_delta_var, ptr);
	}
	else if (p2d->mode_type == P2D_MODE_SPD_COS)
	{
		uint16_t speed = TransFloat(p2d->C.speed);
		pack(speed, ptr);
		uint16_t speed_var = TransFloat(p2d->C.speed_var);
		pack(speed_var, ptr);

		uint16_t cos_amplitude = TransFloat(p2d->C.cos_amplitude);
		pack(cos_amplitude, ptr);
		uint16_t cos_amplitude_var = TransFloat(p2d->C.cos_amplitude_var);
		pack(cos_amplitude_var, ptr);

		uint16_t cos_frequency = TransFloat(p2d->C.cos_frequency);
		pack(cos_frequency, ptr);
		uint16_t cos_frequency_var = TransFloat(p2d->C.cos_frequency_var);
		pack(cos_frequency_var, ptr);
	}
	else
	{
		throw ee::Exception("Particle2DToBin::Pack unknown mode type.");
	}
}

int Particle2DToBin::ComponentSize()
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint16_t) * 2;		// angle
	sz += sizeof(uint16_t) * 2;		// scale
	sz += sizeof(uint32_t) * 4;		// color
	return sz;
}

void Particle2DToBin::PackComponent(const PackParticle2D::Component& comp, 
									uint8_t** ptr)
{
	uint16_t id = comp.node->GetSprID();
	pack(id, ptr);

	uint16_t angle_start = TransRadian(comp.angle_start);
	pack(angle_start, ptr);
	uint16_t angle_end = TransRadian(comp.angle_end);
	pack(angle_end, ptr);

	uint16_t scale_start = TransFloatX100(comp.scale_start);
	pack(scale_start, ptr);
	uint16_t scale_end = TransFloatX100(comp.scale_end);
	pack(scale_end, ptr);

	uint32_t mul_col_begin = comp.mul_col_begin;
	pack(mul_col_begin, ptr);
	uint32_t mul_col_end = comp.mul_col_end;
	pack(mul_col_end, ptr);

	uint32_t add_col_begin = comp.add_col_begin;
	pack(add_col_begin, ptr);
	uint32_t add_col_end = comp.add_col_end;
	pack(add_col_end, ptr);
}

}