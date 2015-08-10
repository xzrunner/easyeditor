#include "FileAdapter.h"

#include <fstream>
#include <json/json.h>
#include <drag2d.h>

namespace eparticle3d
{

void FileAdapter::load(const char* filename, int version)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (version == 0) {
		min_life = value["min_life"].asInt();
		max_life = value["max_life"].asInt();

		min_spd = value["min_spd"].asInt();
		max_spd = value["max_spd"].asInt();

		min_angular_spd = value["min_angular_spd"].asInt();
		max_angular_spd = value["max_angular_spd"].asInt();
		
	} else {
		int c = value["life"]["center"].asInt();
		int o = value["life"]["offset"].asInt();
		min_life = c - o;
		max_life = c + o;

		c = value["speed"]["center"].asInt();
		o = value["speed"]["offset"].asInt();
		min_spd = c - o;
		max_spd = c + o;

		c = value["angular_speed"]["center"].asInt();
		o = value["angular_speed"]["offset"].asInt();
		min_angular_spd = c - o;
		max_angular_spd = c + o;
	}

	int c = value["linear_acc"]["center"].asInt();
	int o = value["linear_acc"]["offset"].asInt();
	min_linear_acc = c - o;
	max_linear_acc = c + o;

	c = value["disturbance_radius"]["center"].asInt();
	o = value["disturbance_radius"]["offset"].asInt();
	min_dis_region = c - o;
	max_dis_region = c + o;

	c = value["disturbance_spd"]["center"].asInt();
	o = value["disturbance_spd"]["offset"].asInt();
	min_dis_spd = c - o;
	max_dis_spd = c + o;

	name = value["name"].asString();
	package = value["package"].asString();
	count = value["count"].asInt();
	layer = value["layer"].asInt();
	emission_time = value["emission_time"].asInt();
	min_hori = value["min_hori"].asInt();
	max_hori = value["max_hori"].asInt();
	min_vert = value["min_vert"].asInt();
	max_vert = value["max_vert"].asInt();
	gravity = value["gravity"].asInt();
	inertia = value["inertia"].asInt();
	fadeout_time = value["fadeout_time"].asInt();
	bounce = value["bounce"].asBool();
	additive_blend = value["additive_blend"].asBool();
	start_radius = value["start_radius"].asInt();
	start_radius_3d = value["start_radius_3d"].asBool();
	
	orient_to_movement = value["orient_to_movement"].asBool();
	orient_to_parent = value["orient_to_parent"].asBool();

	std::string dir = d2d::FilenameTools::getFileDir(filename);
	int i = 0;
	Json::Value childValue = value["components"][i++];
	while (!childValue.isNull()) {
		Child child;

		child.filepath = childValue["filepath"].asString();
		child.filepath = d2d::FilenameTools::getAbsolutePath(dir, child.filepath);

		child.bind_filepath = childValue["bind ps filepath"].asString();
		child.bind_filepath = d2d::FilenameTools::getAbsolutePath(dir, child.bind_filepath);

		child.name = childValue["name"].asString();
		if (version == 0) {
			child.start_scale = childValue["start_scale"].asInt();
			child.end_scale = childValue["end_scale"].asInt();
			child.min_rotate = childValue["min_rotate"].asInt();
			child.max_rotate = childValue["max_rotate"].asInt();
		} else {
			child.start_scale = childValue["scale"]["start"].asInt();
			child.end_scale = childValue["scale"]["end"].asInt();
			if (!childValue["rotate"]["max"].isNull()) {
				child.min_rotate = childValue["rotate"]["min"].asInt();
				child.max_rotate = childValue["rotate"]["max"].asInt();
			} else {
				int c_rotate = childValue["rotate"]["center"].asInt(),
					d_rotate = childValue["rotate"]["offset"].asInt();
				child.min_rotate = c_rotate - d_rotate;
				child.max_rotate = c_rotate + d_rotate;
			}
		}

		if (!childValue["mul_col"].isNull()) {
			child.mul_col.r = childValue["mul_col"]["r"].asDouble();
			child.mul_col.g = childValue["mul_col"]["g"].asDouble();
			child.mul_col.b = childValue["mul_col"]["b"].asDouble();
		} else {
			child.mul_col.r = child.mul_col.g = child.mul_col.b = 1;
		}

		if (!childValue["add_col"].isNull()) {
			child.add_col.r = childValue["add_col"]["r"].asDouble();
			child.add_col.g = childValue["add_col"]["g"].asDouble();
			child.add_col.b = childValue["add_col"]["b"].asDouble();
		} else {
			child.add_col.r = child.add_col.g = child.add_col.b = 0;
		}

		if (!childValue["alpha"].isNull()) {
			child.start_alpha = childValue["alpha"]["start"].asInt();
			child.end_alpha = childValue["alpha"]["end"].asInt();
		} else {
			child.start_alpha = child.end_alpha = 100;
		}

		child.start_z = childValue["start_z"].asInt();

		children.push_back(child);

		childValue = value["components"][i++];
	}
}

}