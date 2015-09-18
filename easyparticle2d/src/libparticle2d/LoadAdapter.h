#ifndef _EASYPARTICLE2D_LOAD_ADAPTER_H_
#define _EASYPARTICLE2D_LOAD_ADAPTER_H_

#include <drag2d.h>

namespace eparticle2d
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
		float angle_start, angle_end;

		float scale_start, scale_end;

		d2d::Colorf col_mul_start, col_mul_end;
		d2d::Colorf col_add_start, col_add_end;

		std::string filepath;
	}; // Component

public:
	std::string name;

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

	//////////////////////////////////////////////////////////////////////////

	float emission_time;
	int count;

	float life, life_var;

	d2d::Vector position, position_var;

	float direction, direction_var;

	std::vector<Component> components;

}; // LoadAdapter

}

#endif // _EASYPARTICLE2D_LOAD_ADAPTER_H_