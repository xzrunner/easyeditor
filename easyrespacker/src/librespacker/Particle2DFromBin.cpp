#include "Particle2DFromBin.h"
#include "UnpackNodeFactory.h"
#include "pack_unpack.h"

#include <ps_2d.h>
#include "spritepack.h"

namespace erespacker
{

int Particle2DFromBin::Size(const PackParticle2D* p2d)
{
	int sz = 0;
	sz += SIZEOF_PARTICLE2D;
	sz += p2d->components.size() * SIZEOF_P2D_SYMBOL;
	return sz;
}

void Particle2DFromBin::Unpack(uint8_t** ptr, PackParticle2D* p2d)
{
	uint16_t sz;
	unpack(sz, ptr);

	// components
	p2d->components.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		UnpackComponent(ptr, p2d);
	}

	// body

	uint16_t emission_time, count;
	unpack(emission_time, ptr);
	unpack(count, ptr);
	p2d->emission_time = TransTime(emission_time);
	p2d->count = count;

	uint16_t life, life_var;
	unpack(life, ptr);
	unpack(life_var, ptr);
	p2d->life = TransTime(life);
	p2d->life_var = TransTime(life_var);

	uint16_t position_x, position_y, position_var_x, position_var_y;
	unpack(position_x, ptr);
	unpack(position_y, ptr);
	unpack(position_var_x, ptr);
	unpack(position_var_y, ptr);
	p2d->position.x = position_x;
	p2d->position.y = position_y;
	p2d->position_var.x = position_var_x;
	p2d->position_var.y = position_var_y;

	uint16_t direction, direction_var;
	unpack(direction, ptr);
	unpack(direction_var, ptr);
	p2d->direction = TransDegree(direction);
	p2d->direction_var = TransDegree(direction_var);

	uint8_t mode_type;
	unpack(mode_type, ptr);
	p2d->mode_type = mode_type;

	if (p2d->mode_type == P2D_MODE_GRAVITY) 
	{
		uint16_t gravity_x, gravity_y;
		unpack(gravity_x, ptr);
		unpack(gravity_y, ptr);
		p2d->A.gravity.x = gravity_x;
		p2d->A.gravity.y = gravity_y;

		uint16_t speed, speed_var;
		unpack(speed, ptr);
		unpack(speed_var, ptr);
		p2d->A.speed = speed;
		p2d->A.speed_var = speed_var;

		uint16_t tangential_accel, tangential_accel_var;
		unpack(tangential_accel, ptr);
		unpack(tangential_accel_var, ptr);
		p2d->A.tangential_accel = tangential_accel;
		p2d->A.tangential_accel_var = tangential_accel_var;

		uint16_t radial_accel, radial_accel_var;
		unpack(radial_accel, ptr);
		unpack(radial_accel_var, ptr);
		p2d->A.radial_accel = radial_accel;
		p2d->A.radial_accel_var = radial_accel_var;

		uint8_t rotation_is_dir;
		unpack(rotation_is_dir, ptr);
		p2d->A.rotation_is_dir = TransBool(rotation_is_dir);
	}
	else if (p2d->mode_type == P2D_MODE_RADIUS)
	{
		uint16_t start_radius, start_radius_var;
		unpack(start_radius, ptr);
		unpack(start_radius_var, ptr);
		p2d->B.start_radius = start_radius;
		p2d->B.start_radius_var = start_radius_var;

		uint16_t end_radius, end_radius_var;
		unpack(end_radius, ptr);
		unpack(end_radius_var, ptr);
		p2d->B.end_radius = end_radius;
		p2d->B.end_radius_var = end_radius_var;

		uint16_t direction_delta, direction_delta_var;
		unpack(direction_delta, ptr);
		unpack(direction_delta_var, ptr);
		p2d->B.direction_delta = direction_delta;
		p2d->B.direction_delta_var = direction_delta_var;
	}
	else if (p2d->mode_type == P2D_MODE_SPD_COS)
	{
		uint16_t speed, speed_var;
		unpack(speed, ptr);
		unpack(speed_var, ptr);
		p2d->C.speed = speed;
		p2d->C.speed_var = speed_var;

		uint16_t cos_amplitude, cos_amplitude_var;
		unpack(cos_amplitude, ptr);
		unpack(cos_amplitude_var, ptr);
		p2d->C.cos_amplitude = cos_amplitude;
		p2d->C.cos_amplitude_var = cos_amplitude_var;

		uint16_t cos_frequency, cos_frequency_var;
		unpack(cos_frequency, ptr);
		unpack(cos_frequency_var, ptr);
		p2d->C.cos_frequency = cos_frequency;
		p2d->C.cos_frequency_var = cos_frequency_var;
	}
	else
	{
		throw ee::Exception("Particle2DFromBin::Unpack unknown mode type.");

	}
}

void Particle2DFromBin::UnpackComponent(uint8_t** ptr, PackParticle2D* p2d)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	p2d->components.push_back(PackParticle2D::Component());
	PackParticle2D::Component& comp = p2d->components[p2d->components.size() - 1];

	uint16_t id;
	unpack(id, ptr);
	comp.node = factory->Query(id);
	if (!comp.node) {
		factory->AddUnassigned(id, &comp.node);
	}

	uint16_t angle_start, angle_end;
	unpack(angle_start, ptr);
	unpack(angle_end, ptr);
	comp.angle_start = TransDegree(angle_start);
	comp.angle_end = TransDegree(angle_end);

	uint16_t scale_start, scale_end;
	unpack(scale_start, ptr);
	unpack(scale_end, ptr);
	comp.scale_start = TransFloatX100(scale_start);
	comp.scale_end = TransFloatX100(scale_end);

	uint32_t mul_col_begin, mul_col_end,
		add_col_begin, add_col_end;
	unpack(mul_col_begin, ptr);
	unpack(mul_col_end, ptr);
	unpack(add_col_begin, ptr);
	unpack(add_col_end, ptr);
	comp.mul_col_begin = mul_col_begin;
	comp.mul_col_end = mul_col_end;
	comp.add_col_begin = add_col_begin;
	comp.add_col_end = add_col_end;
}

}