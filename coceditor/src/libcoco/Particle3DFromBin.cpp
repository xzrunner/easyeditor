#include "Particle3DFromBin.h"
#include "UnpackNodeFactory.h"
#include "tools.h"
#include "spritepack.h"

namespace libcoco
{

int Particle3DFromBin::Size(const PackParticle3D* p3d)
{
	return 0;
}

void Particle3DFromBin::Unpack(uint8_t** ptr, PackParticle3D* p3d)
{
	uint16_t sz;
	unpack(sz, ptr);

	// components
	p3d->components.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		UnpackComponent(ptr, p3d);
	}

	// body

	uint16_t emission_time, count;
	unpack(emission_time, ptr);
	unpack(count, ptr);
	p3d->emission_time = TransTime(emission_time);
	p3d->count = count;

	uint16_t life, life_var;
	unpack(life, ptr);
	unpack(life_var, ptr);
	p3d->life = TransTime(life);
	p3d->life_var = TransTime(life_var);

	uint16_t hori, hori_var, vert, vert_var;
	unpack(hori, ptr);
	unpack(hori_var, ptr);
	unpack(vert, ptr);
	unpack(vert_var, ptr);
	p3d->hori = TransDegree(hori);
	p3d->hori_var = TransDegree(hori_var);
	p3d->vert = TransDegree(vert);
	p3d->vert_var = TransDegree(vert_var);

	uint16_t spd, spd_var, angular_spd, angular_spd_var;
	unpack(spd, ptr);
	unpack(spd_var, ptr);
	unpack(angular_spd, ptr);
	unpack(angular_spd_var, ptr);
	p3d->spd = spd;
	p3d->spd_var = spd_var;
	p3d->angular_spd = TransDegree(angular_spd);
	p3d->angular_spd_var = TransDegree(angular_spd_var);

	uint16_t dis_region, dis_region_var, dis_spd, dis_spd_var;
	unpack(dis_region, ptr);
	unpack(dis_region_var, ptr);
	unpack(dis_spd, ptr);
	unpack(dis_spd_var, ptr);
	p3d->dis_region = dis_region;
	p3d->dis_region_var = dis_region_var;
	p3d->dis_spd = dis_spd;
	p3d->dis_spd_var = dis_spd_var;

	uint16_t gravity;
	unpack(gravity, ptr);
	p3d->gravity = gravity;

	uint16_t linear_acc, linear_acc_var;
	unpack(linear_acc, ptr);
	unpack(linear_acc_var, ptr);
	p3d->linear_acc = linear_acc;
	p3d->linear_acc_var = linear_acc_var;
	
	uint16_t fadeout_time;
	unpack(fadeout_time, ptr);
	p3d->fadeout_time = TransTime(fadeout_time);

	uint8_t bounce;
	unpack(bounce, ptr);
	p3d->bounce = TransBool(bounce);

	uint16_t start_radius;
	unpack(start_radius, ptr);
	p3d->start_radius = start_radius;

	uint8_t is_start_radius_3d;
	unpack(is_start_radius_3d, ptr);
	p3d->is_start_radius_3d = TransBool(is_start_radius_3d);

	uint8_t orient_to_movement;
	unpack(orient_to_movement, ptr);
	p3d->orient_to_movement = TransBool(orient_to_movement);
}

void Particle3DFromBin::UnpackComponent(uint8_t** ptr, PackParticle3D* p3d)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	p3d->components.push_back(PackParticle3D::Component());
	PackParticle3D::Component& comp = p3d->components[p3d->components.size() - 1];

	uint16_t id;
	unpack(id, ptr);
	comp.node = factory->Query(id);
	if (!comp.node) {
		factory->AddUnassigned(id, &comp.node);
	}

	uint16_t scale_start, scale_end;
	unpack(scale_start, ptr);
	unpack(scale_end, ptr);
	comp.scale_start = TransFloatX100(scale_start);
	comp.scale_end = TransFloatX100(scale_end);

	uint16_t angle, angle_var;
	unpack(angle, ptr);
	unpack(angle_var, ptr);
	comp.angle = angle;
	comp.angle_var = angle_var;

	uint32_t col_mul, col_add;
	unpack(col_mul, ptr);
	unpack(col_add, ptr);
	comp.col_mul = col_mul;
	comp.col_add = col_add;

	uint16_t alpha_start, alpha_end;
	unpack(alpha_start, ptr);
	unpack(alpha_end, ptr);
	comp.alpha_start = alpha_start;
	comp.alpha_end = alpha_end;
}

float Particle3DFromBin::TransTime(int time)
{
	return time * 0.001f;
}

float Particle3DFromBin::TransDegree(int deg)
{
	return deg * d2d::TRANS_DEG_TO_RAD;
}

float Particle3DFromBin::TransFloatX100(int f)
{
	return f * 0.01f;
}

bool Particle3DFromBin::TransBool(int b)
{
	return b == 0 ? false : true;
}

}