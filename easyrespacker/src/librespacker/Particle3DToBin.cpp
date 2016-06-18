#include "Particle3DToBin.h"
#include "pack_unpack.h"

#include <spritepack.h>

namespace erespacker
{

int Particle3DToBin::Size(const PackParticle3D* p3d)
{
	int sz = 0;
	sz += sizeof(uint16_t);						// id
	sz += sizeof(uint8_t);						// type
	sz += sizeof(uint16_t);						// component size
	sz += ComponentSize() * p3d->components.size();
	sz += sizeof(uint8_t);						// static_mode
	sz += sizeof(uint16_t) * 2;					// emission_time, count
	sz += sizeof(uint16_t) * 2;					// life
	sz += sizeof(uint16_t) * 4;					// dir
	sz += sizeof(uint16_t) * 2;					// radial spd
	sz += sizeof(uint16_t) * 2;					// tangential spd
	sz += sizeof(uint16_t) * 2;					// angular spd
	sz += sizeof(uint16_t) * 4;					// dis
	sz += sizeof(uint16_t);						// gravity
	sz += sizeof(uint16_t) * 2;					// acc
	sz += sizeof(uint16_t);						// fadeout_time
	sz += sizeof(uint8_t);						// ground
	sz += sizeof(uint16_t) * 2;					// start position
	sz += sizeof(uint8_t);						// orient_to_movement
	// todo struct ps_vec3 dir
	return sz;
}

void Particle3DToBin::Pack(const PackParticle3D* p3d, uint8_t** ptr)
{
	uint16_t id = p3d->GetSprID();
	pack(id, ptr);

	uint8_t type = TYPE_PARTICLE3D;
	pack(type, ptr);

	// components
	uint16_t sz = p3d->components.size();
	pack(sz, ptr);
	for (int i = 0; i < sz; ++i) {
		PackComponent(p3d->components[i], ptr);
	}

	// body

	uint8_t static_mode = TransBool(p3d->static_mode);
	pack(static_mode, ptr);

	uint16_t emission_time = TransTime(p3d->emission_time);
	pack(emission_time, ptr);
	uint16_t count = p3d->count;
	pack(count, ptr);

	uint16_t life = TransTime(p3d->life);
	pack(life, ptr);
	uint16_t life_var = TransTime(p3d->life_var);
	pack(life_var, ptr);

	int16_t hori = TransRadian(p3d->hori);
	pack(hori, ptr);
	uint16_t hori_var = TransRadian(p3d->hori_var);
	pack(hori_var, ptr);
	int16_t vert = TransRadian(p3d->vert);
	pack(vert, ptr);
	uint16_t vert_var = TransRadian(p3d->vert_var);
	pack(vert_var, ptr);

	uint16_t radial_spd = TransFloat(p3d->radial_spd);
	pack(radial_spd, ptr);
	uint16_t radial_spd_var = TransFloat(p3d->radial_spd_var);
	pack(radial_spd_var, ptr);

	int16_t tangential_spd = TransFloat(p3d->tangential_spd);
	pack(tangential_spd, ptr);
	uint16_t tangential_spd_var = TransFloat(p3d->tangential_spd_var);
	pack(tangential_spd_var, ptr);

	int16_t angular_spd = TransRadian(p3d->angular_spd);
	pack(angular_spd, ptr);
	uint16_t angular_spd_var = TransRadian(p3d->angular_spd_var);
	pack(angular_spd_var, ptr);

	uint16_t dis_region = TransFloat(p3d->dis_region);
	pack(dis_region, ptr);
	uint16_t dis_region_var = TransFloat(p3d->dis_region_var);
	pack(dis_region_var, ptr);
	uint16_t dis_spd = TransFloat(p3d->dis_spd);
	pack(dis_spd, ptr);
	uint16_t dis_spd_var = TransFloat(p3d->dis_spd_var);
	pack(dis_spd_var, ptr);

	int16_t gravity = TransFloat(p3d->gravity);
	pack(gravity, ptr);

	int16_t linear_acc = TransFloat(p3d->linear_acc);
	pack(linear_acc, ptr);
	uint16_t linear_acc_var = TransFloat(p3d->linear_acc_var);
	pack(linear_acc_var, ptr);

	uint16_t fadeout_time = TransTime(p3d->fadeout_time);
	pack(fadeout_time, ptr);

	uint8_t ground = p3d->ground;
	pack(ground, ptr);

	uint16_t start_radius = TransFloat(p3d->start_radius);
	pack(start_radius, ptr);
	int16_t start_height = TransFloat(p3d->start_height);
	pack(start_height, ptr);

	uint8_t orient_to_movement = TransBool(p3d->orient_to_movement);
	pack(orient_to_movement, ptr);
}

int Particle3DToBin::ComponentSize()
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint16_t);			// count
	sz += sizeof(uint16_t) * 2;		// scale
	sz += sizeof(uint16_t) * 2;		// angle
	sz += sizeof(uint32_t) * 2;		// color
	sz += sizeof(uint16_t) * 2;		// alpha
	return sz;
}

void Particle3DToBin::PackComponent(const PackParticle3D::Component& comp, 
									uint8_t** ptr)
{
	uint16_t id = comp.node->GetSprID();
	pack(id, ptr);

	uint16_t count = comp.count;
	pack(count, ptr);

	uint16_t scale_start = TransFloatX100(comp.scale_start);
	pack(scale_start, ptr);
	uint16_t scale_end = TransFloatX100(comp.scale_end);
	pack(scale_end, ptr);

	int16_t angle = TransFloat(comp.angle);
	pack(angle, ptr);
	uint16_t angle_var = TransFloat(comp.angle_var);
	pack(angle_var, ptr);

	uint32_t col_mul = comp.col_mul;
	pack(col_mul, ptr);
	uint32_t col_add = comp.col_add;
	pack(col_add, ptr);

	uint16_t alpha_start = comp.alpha_start;
	pack(alpha_start, ptr);
	uint16_t alpha_end = comp.alpha_end;
	pack(alpha_end, ptr);
}

}