#include "LoadAdapter.h"

#include <ee/FileHelper.h>
#include <ee/JsonSerializer.h>

#include <fstream>

namespace etrail
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

	mode = value["mode"].asInt();

	count = value["count"].asDouble();

	life_begin = value["life_begin"].asDouble() * 0.001f;
	life_offset = value["life_offset"].asDouble() * 0.001f;

	fadeout_time = value["fadeout_time"].asDouble() * 0.001f;

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	int idx = 0;
	Json::Value comp_val = value["components"][idx++];
	while (!comp_val.isNull()) {
		if (mode == T2D_MODE_IMAGE) {
			LoadImageComp(dir, comp_val);
		} else {
			assert(mode == T2D_MODE_SHAPE);
			LoadShapeComp(comp_val);
		}
		comp_val = value["components"][idx++];
	}
}

void LoadAdapter::LoadImageComp(const std::string& dir, const Json::Value& comp_val)
{
	CompImage comp;

	comp.filepath = comp_val["filepath"].asString();
	comp.filepath = ee::FileHelper::GetAbsolutePath(dir, comp.filepath);

	comp.scale_begin = comp_val["scale"]["start"].asDouble() * 0.01f;
	comp.scale_end = comp_val["scale"]["end"].asDouble() * 0.01f;

	ee::JsonSerializer::Load(comp_val["mul_col_begin"], comp.mul_col_begin);
	ee::JsonSerializer::Load(comp_val["mul_col_end"], comp.mul_col_end);
	ee::JsonSerializer::Load(comp_val["add_col_begin"], comp.add_col_begin);
	ee::JsonSerializer::Load(comp_val["add_col_end"], comp.add_col_end);

	comp_images.push_back(comp);
}

void LoadAdapter::LoadShapeComp(const Json::Value& comp_val)
{
	CompShape comp;

	comp.linewidth = comp_val["linewidth"].asDouble();
	comp.acuity = comp_val["acuity"].asDouble() * 0.01f;

	ee::JsonSerializer::Load(comp_val["color_begin"], comp.col_begin);
	ee::JsonSerializer::Load(comp_val["color_begin"], comp.col_end);
	
	comp_shapes.push_back(comp);
}

}