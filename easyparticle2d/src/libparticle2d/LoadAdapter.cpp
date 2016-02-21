#include "LoadAdapter.h"

#include <ee/Exception.h>
#include <ee/Math2D.h>
#include <ee/FileHelper.h>

#include <ps_2d.h>

#include <fstream>

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
		A.gravity.x				= static_cast<float>(value["gravity"]["x"].asDouble());
		A.gravity.y				= static_cast<float>(value["gravity"]["y"].asDouble());

		A.speed					= static_cast<float>(value["speed"]["center"].asDouble());
		A.speed_var				= static_cast<float>(value["speed"]["offset"].asDouble());

		A.tangential_accel		= static_cast<float>(value["tangential_accel"]["center"].asDouble());
		A.tangential_accel_var	= static_cast<float>(value["tangential_accel"]["offset"].asDouble());

		A.radial_accel			= static_cast<float>(value["radial_accel"]["center"].asDouble());
		A.radial_accel_var		= static_cast<float>(value["radial_accel"]["offset"].asDouble());

		A.rotation_is_dir		= value["rotation_is_dir"].asBool();
	}
	else if (mode_type == P2D_MODE_RADIUS) 
	{
		B.start_radius			= static_cast<float>(value["start_radius"]["center"].asDouble());
		B.start_radius_var		= static_cast<float>(value["start_radius"]["offset"].asDouble());

		B.end_radius			= static_cast<float>(value["end_radius"]["center"].asDouble());
		B.end_radius_var		= static_cast<float>(value["end_radius"]["offset"].asDouble());

		B.direction_delta		= static_cast<float>(value["direction_delta"]["center"].asDouble());
		B.direction_delta_var	= static_cast<float>(value["direction_delta"]["offset"].asDouble());		
	}
	else if (mode_type == P2D_MODE_SPD_COS) 
	{
		C.speed					= static_cast<float>(value["speed"]["center"].asDouble());
		C.speed_var				= static_cast<float>(value["speed"]["offset"].asDouble());

		C.cos_amplitude			= static_cast<float>(value["cos_amplitude"]["center"].asDouble());
		C.cos_amplitude_var		= static_cast<float>(value["cos_amplitude"]["offset"].asDouble());

		C.cos_frequency			= static_cast<float>(value["cos_frequency"]["center"].asDouble());
		C.cos_frequency_var		= static_cast<float>(value["cos_frequency"]["offset"].asDouble());
	} 
	else 
	{
		throw ee::Exception("LoadAdapter::Load unknown mode type.");
	}

	emission_time = static_cast<float>(value["emission_time"].asDouble() * 0.001f);

	count = value["count"].asInt();

	life				= static_cast<float>(value["life"]["center"].asDouble() * 0.001f);
	life_var			= static_cast<float>(value["life"]["offset"].asDouble() * 0.001f);

	position.x			= static_cast<float>(value["position_x"]["center"].asDouble());
	position_var.x		= static_cast<float>(value["position_x"]["offset"].asDouble());
	position.y			= static_cast<float>(value["position_y"]["center"].asDouble());
	position_var.y		= static_cast<float>(value["position_y"]["offset"].asDouble());

	direction			= static_cast<float>(value["direction"]["center"].asDouble() * ee::TRANS_DEG_TO_RAD);
	direction_var		= static_cast<float>(value["direction"]["offset"].asDouble() * ee::TRANS_DEG_TO_RAD);

	std::string dir = ee::FileHelper::GetFileDir(filepath);
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
	comp.filepath = ee::FileHelper::GetAbsolutePath(dir, comp.filepath);

	components.push_back(comp);
}

}