#ifndef _SIMP_NODE_PARTICLE3D_H_
#define _SIMP_NODE_PARTICLE3D_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class NodeParticle3d : private cu::Uncopyable
{
public:
	struct Component
	{
		uint32_t sym_id;
		uint16_t count;
		uint16_t scale_start, scale_end;
		uint16_t angle, angle_var;
		uint32_t mul_col_begin, mul_col_end,
			     add_col_begin, add_col_end;
	};

	uint16_t emission_time;
	uint16_t count;

	uint16_t life, life_var;

	uint16_t hori, hori_var;
	uint16_t vert, vert_var;
	
	uint16_t radial_spd, radial_spd_var;
	uint16_t tangential_spd, tangential_spd_var;
	uint16_t angular_spd, angular_spd_var;

	uint16_t dis_region, dis_region_var;
	uint16_t dis_spd, dis_spd_var;

	uint16_t gravity;

	uint16_t linear_acc, linear_acc_var;

	uint16_t fadeout_time;

	uint16_t start_radius,  start_height;

	uint8_t static_mode;
	uint8_t orient_to_movement;
	uint8_t ground;
	uint8_t blend;

	int n;
	Component components[1];

public:
	NodeParticle3d(Allocator& alloc, ImportStream& is);

	static int Size();
	static int ComponentSize();

}; // NodeParticle3d

}

#endif // _SIMP_NODE_PARTICLE3D_H_