#ifndef _S2LOADER_P3D_SYM_LOADER_H_
#define _S2LOADER_P3D_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>
#include <painting2/Color.h>
#include <sprite2/typedef.h>

#include <json/json.h>

namespace s2 { class Particle3dSymbol; class Symbol; class P3dEmitterCfg; }
namespace simp { class NodeParticle3d; }

struct p3d_emitter_cfg;

namespace s2loader
{

class P3dSymLoader : private cu::Uncopyable
{
public:
	P3dSymLoader();

	void Store(const std::shared_ptr<s2::Particle3dSymbol>& sym) const;
	void Store(const std::shared_ptr<s2::P3dEmitterCfg>& p3d_cfg) const;

	void LoadJson(const CU_STR& filepath);	
	void LoadBin(const simp::NodeParticle3d* node);

private:
	void LoadComponent(const CU_STR& dir, const Json::Value& comp_val);

protected:
	virtual s2::SymPtr LoadSymbol(const CU_STR& filepath) const;

public:
	struct Component
	{
		int count;

		uint32_t sym_id;
		CU_STR filepath;

		CU_STR name;

		float scale_start, scale_end;

		float angle, angle_var;

		pt2::Color mul_col_begin, mul_col_end,
			      add_col_begin, add_col_end;

		float alpha_start, alpha_end;

		float start_z;

		Component() 
			: count(0)
			, sym_id(0)
			, scale_start(0)
			, scale_end(0)
			, angle(0)
			, angle_var(0)
			, alpha_start(0)
			, alpha_end(0)
			, start_z(0)
		{
		}

	}; // Component

public:
	CU_STR name;

	bool loop, local;

	bool static_mode;

	int count;
	float emission_time;

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

	float inertia;

	float fadeout_time;

	int ground;

	float start_radius, start_height;

	bool orient_to_movement;
	bool orient_to_parent;

	int blend;

	CU_VEC<Component> components;

}; // P3dSymLoader

}

#endif // _S2LOADER_P3D_SYM_LOADER_H_
