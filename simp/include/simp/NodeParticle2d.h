#ifndef _SIMP_NODE_PARTICLE2D_H_
#define _SIMP_NODE_PARTICLE2D_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class NodeParticle2d : private cu::Uncopyable
{
public:
	struct Component
	{
		uint32_t sym_id;

		uint16_t angle_start, angle_end;
		uint16_t scale_start, scale_end;

		uint32_t mul_col_begin, mul_col_end,
		         add_col_begin, add_col_end;
	};

	union {
		// gravity + tangential + radial
		struct {
			uint16_t gravity[2];

			uint16_t speed, speed_var;

			uint16_t tangential_accel, tangential_accel_var;
			uint16_t radial_accel, radial_accel_var;

			uint8_t rotation_is_dir;
			uint8_t _pad[3];
		} A;

		// radius + rotate
		struct {
			uint16_t start_radius, start_radius_var;
			uint16_t end_radius, end_radius_var;

			uint16_t direction_delta, direction_delta_var;
		} B;

		// tangential spd cos
		struct {
			uint16_t speed, speed_var;

			uint16_t cos_amplitude, cos_amplitude_var;
			uint16_t cos_frequency, cos_frequency_var;
		} C;

	} mode;

	uint16_t emission_time;
	uint16_t count;

	uint16_t life, life_var;

	uint16_t position[2], position_var[2];

	uint16_t direction, direction_var;

	uint8_t mode_type;
	uint8_t _pad[3];

	int n;
	Component components[1];

public:
	NodeParticle2d(Allocator& alloc, ImportStream& is);

	static int Size();
	static int ComponentSize();

}; // NodeParticle2d

}

#endif // _SIMP_NODE_PARTICLE2D_H_