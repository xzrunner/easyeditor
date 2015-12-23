#ifndef _EASYPARTICLE3D_LOAD_ADAPTER_H_
#define _EASYPARTICLE3D_LOAD_ADAPTER_H_

#include <drag2d.h>

namespace eparticle3d
{

class LoadAdapter
{
public:
	void Load(const std::string& filepath);

private:
	void LoadComponent(const std::string& dir, const Json::Value& comp_val);

public:
	struct Component
	{
		std::string filepath;
		std::string bind_filepath;

		std::string name;

		float scale_start, scale_end;

		float angle, angle_var;

		d2d::Colorf col_mul, col_add;
		float alpha_start, alpha_end;

		float start_z;
	};

public:
	std::string name;

	int count;
	float emission_time;

	float life, life_var;

	float hori, hori_var;
	float vert, vert_var;

	float spd, spd_var;
	float angular_spd, angular_spd_var;

	float dis_region, dis_region_var;
	float dis_spd, dis_spd_var;

	float gravity;

	float linear_acc, linear_acc_var;

	float inertia;

	float fadeout_time;

	int ground;

	bool additive_blend;

	float start_radius;
	bool is_start_radius_3d;

	bool orient_to_movement;
	bool orient_to_parent;

	bool loop;

	std::vector<Component> components;

}; // LoadAdapter

}

#endif // _EASYPARTICLE3D_LOAD_ADAPTER_H_