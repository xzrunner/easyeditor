#include "FileAdapterNew.h"

namespace eparticle3d
{

void FileAdapterNew::Load(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();
	package = value["package"].asString();

	count = value["count"].asInt();
	layer = value["layer"].asInt();
	emission_time = value["emission_time"].asInt();

	if (!value["life"].isNull()) {
		life = value["life"]["center"].asInt();
		life_var = value["life"]["offset"].asInt();
	} else {
		int min = value["min_life"].asInt(),
			max = value["max_life"].asInt();
		life = (min + max) * 0.5f;
		life_var = (max - min) * 0.5f;
	}

	min_hori = value["min_hori"].asInt();
	max_hori = value["max_hori"].asInt();
	min_vert = value["min_vert"].asInt();
	max_vert = value["max_vert"].asInt();

	if (!value["speed"].isNull()) {
		spd = value["speed"]["center"].asInt();
		spd_var = value["speed"]["offset"].asInt();
	} else {
		int min, max;
		min = value["min_spd"].asInt();
		max = value["max_spd"].asInt();
		spd = (min + max) * 0.5f;
		spd_var = (max - min) * 0.5f;
	}

	if (!value["angular_speed"].isNull()) {
		angular_spd = value["angular_speed"]["center"].asInt();
		angular_spd_var = value["angular_speed"]["offset"].asInt();
	} else {
		int min, max;
		min = value["min_angular_spd"].asInt();
		max = value["max_angular_spd"].asInt();
		angular_spd = (min + max) * 0.5f;
		angular_spd_var = (max - min) * 0.5f;
	}

	dis_region = value["disturbance_radius"]["center"].asInt();
	dis_region_var = value["disturbance_radius"]["offset"].asInt();

	dis_spd = value["disturbance_spd"]["center"].asInt();
	dis_spd_var = value["disturbance_spd"]["offset"].asInt();

	gravity = value["gravity"].asInt();

	linear_acc = value["linear_acc"]["center"].asInt();
	linear_acc_var = value["linear_acc"]["offset"].asInt();

	inertia = value["inertia"].asInt();

	fadeout_time = value["fadeout_time"].asInt();

	bounce = value["bounce"].asBool();
	additive_blend = value["additive_blend"].asBool();

	start_radius = value["start_radius"].asInt();
	start_radius_3d = value["start_radius_3d"].asBool();

	orient_to_movement = value["orient_to_movement"].asBool();
	orient_to_parent = value["orient_to_parent"].asBool();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	int i = 0;
	Json::Value comp_val = value["components"][i++];
	while (!comp_val.isNull()) {
		LoadComponent(dir, comp_val);
		comp_val = value["components"][i++];
	}
}

void FileAdapterNew::LoadComponent(const std::string& dir, const Json::Value& comp_val)
{
	Component comp;

	comp.filepath = comp_val["filepath"].asString();
	comp.filepath = d2d::FilenameTools::getAbsolutePath(dir, comp.filepath);

	comp.bind_filepath = comp_val["bind ps filepath"].asString();
	comp.bind_filepath = d2d::FilenameTools::getAbsolutePath(dir, comp.bind_filepath);

	comp.name = comp_val["name"].asString();

	if (!comp_val["scale"].isNull()) {
		comp.scale_start = comp_val["scale"]["start"].asInt();
		comp.scale_end = comp_val["scale"]["start"].asInt();
	} else {
		comp.scale_start = comp_val["start_scale"].asInt();
		comp.scale_end = comp_val["end_scale"].asInt();
	}

	if (!comp_val["rotate"].isNull()) {
		comp.angle = comp_val["rotate"]["center"].asInt();
		comp.angle_var = comp_val["rotate"]["offset"].asInt();
	} else {
		int min = comp_val["rotate"]["min"].asInt(),
			max = comp_val["rotate"]["max"].asInt();
		comp.angle = (min + max) * 0.5f;
		comp.angle_var = (max - min) * 0.5f;
	}

	if (!comp_val["col_mul"].isNull()) {
		comp.col_mul.r = comp_val["col_mul"]["r"].asDouble();
		comp.col_mul.g = comp_val["col_mul"]["g"].asDouble();
		comp.col_mul.b = comp_val["col_mul"]["b"].asDouble();
	} else {
		comp.col_mul.r = comp.col_mul.g = comp.col_mul.b = 1;
	}

	if (!comp_val["col_add"].isNull()) {
		comp.col_add.r = comp_val["col_add"]["r"].asDouble();
		comp.col_add.g = comp_val["col_add"]["g"].asDouble();
		comp.col_add.b = comp_val["col_add"]["b"].asDouble();
	} else {
		comp.col_add.r = comp.col_add.g = comp.col_add.b = 0;
	}

	if (!comp_val["alpha"].isNull()) {
		comp.alpha_start = comp_val["alpha"]["start"].asInt();
		comp.alpha_end = comp_val["alpha"]["end"].asInt();
	} else {
		comp.alpha_start = comp.alpha_end = 100;
	}

	comp.start_z = comp_val["start_z"].asInt();

	components.push_back(comp);
}

}