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

	name = value["name"].asString();
	package = value["package"].asString();
	count = value["count"].asInt();
	layer = value["layer"].asInt();
	emission_time = value["emission_time"].asInt();
	min_life = value["min_life"].asInt();
	max_life = value["max_life"].asInt();
	min_hori = value["min_hori"].asInt();
	max_hori = value["max_hori"].asInt();
	min_vert = value["min_vert"].asInt();
	max_vert = value["max_vert"].asInt();
	min_spd = value["min_spd"].asInt();
	max_spd = value["max_spd"].asInt();
	min_angular_spd = value["min_angular_spd"].asInt();
	max_angular_spd = value["max_angular_spd"].asInt();
	gravity = value["gravity"].asInt();
	inertia = value["inertia"].asInt();
	fadeout_time = value["fadeout_time"].asInt();
	bounce = value["bounce"].asBool();
	additive_blend = value["additive_blend"].asBool();
	start_radius = value["start_radius"].asInt();
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
			int c_rotate = childValue["rotate"]["center"].asInt(),
				d_rotate = childValue["rotate"]["offset"].asInt();
			child.min_rotate = c_rotate - d_rotate;
			child.max_rotate = c_rotate + d_rotate;
		}
		child.start_z = childValue["start_z"].asInt();

		children.push_back(child);

		childValue = value["components"][i++];
	}
}

}