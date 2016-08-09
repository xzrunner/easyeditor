#ifndef _EASYRESPACKER_PACK_PARTICLE2D_H_
#define _EASYRESPACKER_PACK_PARTICLE2D_H_

#include "IPackNode.h"

#include <SM_Vector.h>

namespace erespacker
{

class PackParticle2D : public IPackNode
{
public:
	struct Component
	{
		float angle_start, angle_end;

		float scale_start, scale_end;

		uint32_t mul_col_begin, mul_col_end;
		uint32_t add_col_begin, add_col_end;

		const IPackNode* node;
	};

	float emission_time;
	int count;

	float life, life_var;

	sm::vec2 position, position_var;

	float direction, direction_var;

	std::vector<Component> components;

	int mode_type;

	//////////////////////////////////////////////////////////////////////////
	// A: gravity + tangential + radial
	//////////////////////////////////////////////////////////////////////////
	struct A
	{
		sm::vec2 gravity;

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
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, 
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images);

}; // PackParticle2D

}

#endif // _EASYRESPACKER_PACK_PARTICLE2D_H_