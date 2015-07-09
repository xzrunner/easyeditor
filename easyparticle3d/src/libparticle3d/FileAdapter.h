#ifndef _EASYPARTICLE3D_FILE_ADAPTER_H_
#define _EASYPARTICLE3D_FILE_ADAPTER_H_

#include <string>
#include <vector>

namespace eparticle3d
{

class FileAdapter
{
public:
	virtual void load(const char* filename, int version);

public:
	struct Child
	{
		std::string filepath;
		std::string bind_filepath;

		std::string name;

		float start_scale, end_scale;

		float min_rotate, max_rotate;

		float start_z;
	};

public:
	std::string name;
	std::string package;

	int count;
	int layer;
	float emission_time;

	float min_life, max_life;

	float min_hori, max_hori;
	float min_vert, max_vert;

	float min_spd, max_spd;
	float min_angular_spd, max_angular_spd;

	float min_dis_region, max_dis_region;
	float min_dis_spd, max_dis_spd;

	float gravity;

	float min_linear_acc, max_linear_acc;

	float inertia;

	float fadeout_time;

	bool bounce;

	bool additive_blend;

	float start_radius;
	bool start_radius_3d;

	bool orient_to_movement;

	bool orient_to_parent;

	std::vector<Child> children;

}; // FileAdapter

}

#endif // _EASYPARTICLE3D_FILE_ADAPTER_H_
