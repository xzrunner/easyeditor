#include "LoadAdapter.h"

#include <particle2d.h>

namespace eparticle2d
{

void LoadAdapter::Load(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	mode_type = value["mode_type"].asInt();

	if (mode_type == P2D_MODE_GRAVITY) 
	{
		A.gravity.x				= value["gravity"]["x"].asDouble();
		A.gravity.y				= value["gravity"]["y"].asDouble();

		A.speed					= value["speed"]["center"].asDouble();
		A.speed_var				= value["speed"]["offset"].asDouble();

		A.tangential_accel		= value["tangential_accel"]["center"].asDouble();
		A.tangential_accel_var	= value["tangential_accel"]["offset"].asDouble();

		A.radial_accel			= value["radial_accel"]["center"].asDouble();
		A.radial_accel_var		= value["radial_accel"]["offset"].asDouble();

		A.rotation_is_dir		= value["rotation_is_dir"].asBool();
	}
	else if (mode_type == P2D_MODE_RADIUS) 
	{
		B.start_radius			= value["start_radius"]["center"].asDouble();
		B.start_radius_var		= value["start_radius"]["offset"].asDouble();

		B.end_radius			= value["end_radius"]["center"].asDouble();
		B.end_radius_var		= value["end_radius"]["offset"].asDouble();

		B.direction_delta		= value["direction_delta"]["center"].asDouble();
		B.direction_delta_var	= value["direction_delta"]["offset"].asDouble();		
	}
	else if (mode_type == P2D_MODE_SPD_COS) 
	{
		C.speed					= value["speed"]["center"].asDouble();
		C.speed_var				= value["speed"]["offset"].asDouble();

		C.cos_amplitude			= value["cos_amplitude"]["center"].asDouble();
		C.cos_amplitude_var		= value["cos_amplitude"]["offset"].asDouble();

		C.cos_frequency			= value["cos_frequency"]["center"].asDouble();
		C.cos_frequency_var		= value["cos_frequency"]["offset"].asDouble();
	} 
	else 
	{
		throw d2d::Exception("LoadAdapter::Load unknown mode type.");
	}

	emission_time = value["emission_time"].asDouble() * 0.001f;

	count = value["count"].asInt();

	life				= value["life"]["center"].asDouble() * 0.001f;
	life_var			= value["life"]["offset"].asDouble() * 0.001f;

	position.x			= value["position"]["center"]["x"].asDouble();
	position.y			= value["position"]["center"]["y"].asDouble();
	position_var.x		= value["position"]["offset"]["x"].asDouble();
	position_var.y		= value["position"]["offset"]["y"].asDouble();

	direction			= value["direction"]["center"].asDouble() * d2d::TRANS_DEG_TO_RAD;
	direction_var		= value["direction"]["offset"].asDouble() * d2d::TRANS_DEG_TO_RAD;

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	int idx = 0;
	Json::Value comp_val = value["components"][idx++];
	while (!comp_val.isNull()) {
		LoadComponent(dir, comp_val);
		comp_val = value["components"][idx++];
	}
}

void LoadAdapter::LoadComponent(const std::string& dir, const Json::Value& comp_val)
{
	Component comp;

	comp.angle_start		= comp_val["angle"]["start"].asDouble();
	comp.angle_end			= comp_val["angle"]["end"].asDouble();

	comp.scale_start		= comp_val["scale"]["start"].asDouble();
	comp.scale_end			= comp_val["scale"]["end"].asDouble();

	comp.col_mul_start.r	= comp_val["col_mul_start"]["r"].asDouble();
	comp.col_mul_start.g	= comp_val["col_mul_start"]["g"].asDouble();
	comp.col_mul_start.b	= comp_val["col_mul_start"]["b"].asDouble();
	comp.col_mul_start.a	= comp_val["col_mul_start"]["a"].asDouble();

	comp.col_mul_end.r		= comp_val["col_mul_end"]["r"].asDouble();
	comp.col_mul_end.g		= comp_val["col_mul_end"]["g"].asDouble();
	comp.col_mul_end.b		= comp_val["col_mul_end"]["b"].asDouble();
	comp.col_mul_end.a		= comp_val["col_mul_end"]["a"].asDouble();

	comp.col_add_start.r	= comp_val["col_add_start"]["r"].asDouble();
	comp.col_add_start.g	= comp_val["col_add_start"]["g"].asDouble();
	comp.col_add_start.b	= comp_val["col_add_start"]["b"].asDouble();

	comp.col_add_end.r		= comp_val["col_add_end"]["r"].asDouble();
	comp.col_add_end.g		= comp_val["col_add_end"]["g"].asDouble();
	comp.col_add_end.b		= comp_val["col_add_end"]["b"].asDouble();

	comp.filepath = comp_val["filepath"].asString();
	comp.filepath = d2d::FilenameTools::getAbsolutePath(dir, comp.filepath);

	components.push_back(comp);
}

}