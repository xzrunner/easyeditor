#ifndef _LIBRESPACKER_PACK_PARTICLE2D_H_
#define _LIBRESPACKER_PACK_PARTICLE2D_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace librespacker
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
	struct A
	{
		d2d::Vector gravity;

		float speed, speed_var;

		float tangential_accel, tangential_accel_var;
		float radial_accel, radial_accel_var;

		bool rotation_is_dir;
	};

	//////////////////////////////////////////////////////////////////////////
	// B: radius + rotate
	//////////////////////////////////////////////////////////////////////////
	struct B
	{
		float start_radius, start_radius_var;
		float end_radius, end_radius_var;

		float direction_delta, direction_delta_var;
	};

	//////////////////////////////////////////////////////////////////////////
	// C: tangential spd cos
	//////////////////////////////////////////////////////////////////////////
	struct C
	{
		float speed, speed_var;

		float cos_amplitude, cos_amplitude_var;
		float cos_frequency, cos_frequency_var;
	};

	A A;
	B B;
	C C;

public:
	PackParticle2D() {}
	PackParticle2D(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen, 
		const d2d::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, 
		const d2d::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, const std::vector<d2d::Image*>& images);

}; // PackParticle2D

}

#endif // _LIBRESPACKER_PACK_PARTICLE2D_H_