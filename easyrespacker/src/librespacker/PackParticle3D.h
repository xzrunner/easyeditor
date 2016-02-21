#ifndef _EASYRESPACKER_PACK_PARTICLE3D_H_
#define _EASYRESPACKER_PACK_PARTICLE3D_H_

#include "IPackNode.h"



namespace erespacker
{

class PackParticle3D : public IPackNode
{
public:
	struct Component
	{
		float scale_start, scale_end;

		float angle, angle_var;

		uint32_t col_mul, col_add;
		uint8_t alpha_start, alpha_end;

		const IPackNode* node;
	};

	float emission_time;
	int count;

	float life, life_var;

	float hori, hori_var;
	float vert, vert_var;

	float radial_spd, radial_spd_var;
	float tangential_spd, tangential_spd_var;
	float angular_spd, angular_spd_var;

	float dis_region, dis_region_var;
	float dis_spd, dis_spd_var;

	float gravity;

	float linear_acc, linear_acc_var;

	float fadeout_time;

	int ground;

	float start_radius;
	float start_height;

	bool orient_to_movement;

	std::vector<Component> components;

public:
	PackParticle3D() {}
	PackParticle3D(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::Image*>& images);

};	// PackParticle3D 

}

#endif // _EASYRESPACKER_PACK_PARTICLE3D_H_