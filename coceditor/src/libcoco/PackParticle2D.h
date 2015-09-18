#ifndef _LIBCOCO_PACK_PARTICLE2D_H_
#define _LIBCOCO_PACK_PARTICLE2D_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace libcoco
{

class PackParticle2D : public IPackNode
{
public:
	struct Component
	{
		float angle_start, angle_end;

		float scale_start, scale_end;

		uint32_t col_mul_start, col_mul_end;
		uint32_t col_add_start, col_add_end;

		const IPackNode* node;
	};

	int capacity;

	float emission_time;
	int count;

	float life, life_var;

	d2d::Vector position, position_var;

	float direction, direction_var;

	std::vector<Component> components;

	int mode_type;

	//////////////////////////////////////////////////////////////////////////
	// A: gravity + tangential + radial
	//////////////////////////////////////////////////////////////////////////

	d2d::Vector a_gravity;

	float a_speed, a_speed_var;

	float a_tangential_accel, a_tangential_accel_var;
	float a_radial_accel, a_radial_accel_var;

	bool a_rotation_is_dir;

	//////////////////////////////////////////////////////////////////////////
	// B: radius + rotate
	//////////////////////////////////////////////////////////////////////////

	float b_start_radius, b_start_radius_var;
	float b_end_radius, b_end_radius_var;

	float b_direction_delta, b_direction_delta_var;

	//////////////////////////////////////////////////////////////////////////
	// C: tangential spd cos
	//////////////////////////////////////////////////////////////////////////

	float c_speed, c_speed_var;

	float c_cos_amplitude, c_cos_amplitude_var;
	float c_cos_frequency, c_cos_frequency_var;

public:
	PackParticle2D() {}
	PackParticle2D(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen, const d2d::TexturePacker& tp) const;
	virtual void UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const d2d::TexturePacker& tp) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, const std::vector<d2d::Image*>& images);

}; // PackParticle2D

}

#endif // _LIBCOCO_PACK_PARTICLE2D_H_