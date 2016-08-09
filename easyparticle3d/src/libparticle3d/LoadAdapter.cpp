#include "LoadAdapter.h"

#include <ee/Math2D.h>
#include <ee/FileHelper.h>
#include <ee/JsonSerializer.h>

#include <ps_3d.h>

#include <fstream>

namespace eparticle3d
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

	loop = value["loop"].asBool();
	local = value["local"].asBool();

	static_mode = value["static_mode"].asBool();

	count = value["count"].asInt();
	emission_time = value["emission_time"].asInt() * 0.001f;

	if (!value["life"].isNull()) {
		life = static_cast<float>(value["life"]["center"].asInt());
		life_var = static_cast<float>(value["life"]["offset"].asInt());
	} else {
		int min = value["min_life"].asInt(),
			max = value["max_life"].asInt();
		life = (min + max) * 0.5f;
		life_var = (max - min) * 0.5f;
	}

	int min, max;
	min = value["min_hori"].asInt();
	max = value["max_hori"].asInt();
	hori = (min + max) * 0.5f * SM_DEG_TO_RAD;
	hori_var = (max - min) * 0.5f * SM_DEG_TO_RAD;

	min = value["min_vert"].asInt();
	max = value["max_vert"].asInt();
	vert = (min + max) * 0.5f * SM_DEG_TO_RAD;
	vert_var = (max - min) * 0.5f * SM_DEG_TO_RAD;

	if (!value["radial_speed"].isNull()) {
		radial_spd = static_cast<float>(value["radial_speed"]["center"].asInt());
		radial_spd_var = static_cast<float>(value["radial_speed"]["offset"].asInt());
	} else if (!value["speed"].isNull()) {
		radial_spd = static_cast<float>(value["speed"]["center"].asInt());
		radial_spd_var = static_cast<float>(value["speed"]["offset"].asInt());
	} else {
		int min, max;
		min = value["min_spd"].asInt();
		max = value["max_spd"].asInt();
		radial_spd = (min + max) * 0.5f;
		radial_spd_var = (max - min) * 0.5f;
	}

	tangential_spd = static_cast<float>(value["tangential_spd"]["center"].asInt());
	tangential_spd_var = static_cast<float>(value["tangential_spd"]["offset"].asInt());

	// todo
	radial_spd *= 0.25f;
	radial_spd_var *= 0.25f;

	if (!value["angular_speed"].isNull()) {
		angular_spd = value["angular_speed"]["center"].asInt() * SM_DEG_TO_RAD;
		angular_spd_var = value["angular_speed"]["offset"].asInt() * SM_DEG_TO_RAD;
	} else {
		int min, max;
		min = value["min_angular_spd"].asInt();
		max = value["max_angular_spd"].asInt();
		angular_spd = (min + max) * 0.5f * SM_DEG_TO_RAD;
		angular_spd_var = (max - min) * 0.5f * SM_DEG_TO_RAD;
	}

	dis_region = static_cast<float>(value["disturbance_radius"]["center"].asInt());
	dis_region_var = static_cast<float>(value["disturbance_radius"]["offset"].asInt());

	dis_spd = static_cast<float>(value["disturbance_spd"]["center"].asInt());
	dis_spd_var = static_cast<float>(value["disturbance_spd"]["offset"].asInt());

	gravity = static_cast<float>(value["gravity"].asInt());
	// todo
	gravity *= 0.3f;

	linear_acc = static_cast<float>(value["linear_acc"]["center"].asInt());
	linear_acc_var = static_cast<float>(value["linear_acc"]["offset"].asInt());

	inertia = static_cast<float>(value["inertia"].asInt());

	fadeout_time = value["fadeout_time"].asInt() * 0.001f;

	if (value["ground"].isNull()) {
		ground = P3D_GROUND_WITH_BOUNCE;
	} else {
		ground = value["ground"].asInt();
	}

	start_radius = abs(static_cast<float>(value["start_pos"]["radius"].asInt()));
	start_height = static_cast<float>(value["start_pos"]["height"].asInt());

	orient_to_movement = value["orient_to_movement"].asBool();
	orient_to_parent = value["orient_to_parent"].asBool();

	blend = value["blend"].asInt();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	int i = 0;
	Json::Value comp_val = value["components"][i++];
	while (!comp_val.isNull()) {
		LoadComponent(dir, comp_val);
		comp_val = value["components"][i++];
	}
}

void LoadAdapter::LoadComponent(const std::string& dir, const Json::Value& comp_val)
{
	Component comp;

	if (comp_val["count"].isNull()) {
		comp.count = 0;
	} else {
		comp.count = comp_val["count"].asInt();
	}

	comp.filepath = comp_val["filepath"].asString();
	comp.filepath = ee::FileHelper::GetAbsolutePath(dir, comp.filepath);

	if (!comp_val["bind ps filepath"].isNull()) {
		comp.bind_filepath = comp_val["bind ps filepath"].asString();
		if (!comp.bind_filepath.empty()) {
			comp.bind_filepath = ee::FileHelper::GetAbsolutePath(dir, comp.bind_filepath);
		}
	}

	comp.name = comp_val["name"].asString();

	if (!comp_val["scale"].isNull()) {
		comp.scale_start = comp_val["scale"]["start"].asInt();
		comp.scale_end = comp_val["scale"]["end"].asInt();
	} else {
		comp.scale_start = comp_val["start_scale"].asInt();
		comp.scale_end = comp_val["end_scale"].asInt();
	}

	if (!comp_val["rotate"].isNull()) {
		int min = comp_val["rotate"]["min"].asInt(),
			max = comp_val["rotate"]["max"].asInt();
		comp.angle = (min + max) * 0.5f;
		comp.angle_var = (max - min) * 0.5f;
	} else {
		comp.angle = comp.angle_var = 0;
	}

	if (!comp_val["mul_col"].isNull()) {
		comp.mul_col_begin.r = comp_val["mul_col"]["r"].asDouble() * 255;
		comp.mul_col_begin.g = comp_val["mul_col"]["g"].asDouble() * 255;
		comp.mul_col_begin.b = comp_val["mul_col"]["b"].asDouble() * 255;
		comp.mul_col_begin.a = 255;
		comp.mul_col_end = comp.mul_col_begin;
	} else {
		if (!comp_val["mul_col_begin"].isNull()) {
			ee::JsonSerializer::Load(comp_val["mul_col_begin"], comp.mul_col_begin);
		} else {
			comp.mul_col_begin.r = comp.mul_col_begin.g = comp.mul_col_begin.b = comp.mul_col_begin.a = 255;
		}
		if (!comp_val["mul_col_end"].isNull()) {
			ee::JsonSerializer::Load(comp_val["mul_col_end"], comp.mul_col_end);
		} else {
			comp.mul_col_end.r = comp.mul_col_end.g = comp.mul_col_end.b = comp.mul_col_end.a = 255;
		}
	}

	if (!comp_val["add_col"].isNull()) {
		comp.add_col_begin.r = comp_val["add_col"]["r"].asDouble() * 255;
		comp.add_col_begin.g = comp_val["add_col"]["g"].asDouble() * 255;
		comp.add_col_begin.b = comp_val["add_col"]["b"].asDouble() * 255;
		comp.add_col_end = comp.add_col_begin;
	} else {
		if (!comp_val["add_col_begin"].isNull()) {
			ee::JsonSerializer::Load(comp_val["add_col_begin"], comp.add_col_begin);
		} else {
			comp.add_col_begin.r = comp.add_col_begin.g = comp.add_col_begin.b = comp.add_col_begin.a = 0;
		}
		if (!comp_val["add_col_end"].isNull()) {
			ee::JsonSerializer::Load(comp_val["add_col_end"], comp.add_col_end);
		} else {
			comp.add_col_end.r = comp.add_col_end.g = comp.add_col_end.b = comp.add_col_end.a = 0;
		}
	}

	if (!comp_val["alpha"].isNull()) {
		float start = comp_val["alpha"]["start"].asInt() * 0.01f;
		float end = comp_val["alpha"]["end"].asInt() * 0.01f;
		comp.mul_col_begin.a *= start;
		comp.mul_col_end.a *= end;
		comp.alpha_start = 255 * start;
		comp.alpha_end = 255 * end;
	}

	if (!comp_val["alpha2"].isNull()) {
		comp.alpha_start = comp_val["alpha2"]["start"].asInt();
		comp.alpha_end = comp_val["alpha2"]["end"].asInt();
	}

	comp.start_z = comp_val["start_z"].asInt();

	components.push_back(comp);
}

}