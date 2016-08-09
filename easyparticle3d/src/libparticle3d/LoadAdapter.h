#ifndef _EASYPARTICLE3D_LOAD_ADAPTER_H_
#define _EASYPARTICLE3D_LOAD_ADAPTER_H_

#include <sprite2/Color.h>

#include <json/json.h>

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
		int count;

		std::string filepath;
		std::string bind_filepath;

		std::string name;

		float scale_start, scale_end;

		float angle, angle_var;

		s2::Color mul_col_begin, mul_col_end,
			      add_col_begin, add_col_end;

		float start_z;
	};

public:
	std::string name;

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

	std::vector<Component> components;

}; // LoadAdapter

}

#endif // _EASYPARTICLE3D_LOAD_ADAPTER_H_