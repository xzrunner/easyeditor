#ifndef _LIBCOCO_PACK_PARTICLE3D_H_
#define _LIBCOCO_PACK_PARTICLE3D_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace libcoco
{

class PackParticle3D : public IPackNode
{
public:
	struct Component
	{
		float scale_start, scale_end;

		float angle, angle_var;

		uint32_t col_mul, col_add;
		float alpha_start, alpha_end;

		const IPackNode* node;
	};

	float emission_time;
	int count;

	float life, life_var;

	float hori, hori_var;
	float vert, vert_var;

	float spd, spd_var;
	float angular_spd, angular_spd_var;

	float dis_region, dis_region_var;
	float dis_spd, dis_spd_var;

	float gravity;

	float linear_acc, linear_acc_var;

	float fadeout_time;

	bool bounce;

	float start_radius;
	bool is_start_radius_3d;

	bool orient_to_movement;
	
	std::vector<Component> components;

public:
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const d2d::TexturePacker& tp) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<d2d::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const d2d::TexturePacker& tp) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<d2d::Image*>& images);

};	// PackParticle3D 

}

#endif // _LIBCOCO_PACK_PARTICLE3D_H_