#include "LRExpandInherit.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <fstream>

namespace edb
{

std::string LRExpandInherit::Command() const
{
	return "lr-expand-inherit";
}

std::string LRExpandInherit::Description() const
{
	return "expand inherit";
}

std::string LRExpandInherit::Usage() const
{
	// lr-expand-inherit e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

int LRExpandInherit::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_file(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void LRExpandInherit::Run(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	Json::Value new_lr_val = lr_val;

	int idx = 0;
	Json::Value layer_val = lr_val["layer"][idx++];
	while (!layer_val.isNull()) {
		if (!layer_val["base filepath"].isNull()) {
			ExtendLayer(dir, layer_val["base filepath"].asString(), idx-1, new_lr_val);
		}
		layer_val = lr_val["layer"][idx++];
	}

	std::string out_path = GetOutputFilepath(filepath);
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(out_path.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, new_lr_val);
	fout.close();
}

std::string LRExpandInherit::GetOutputFilepath(const std::string& filepath) const
{
	std::string out_path = filepath;
	out_path.insert(filepath.find("_lr.json"), "_file");
	return out_path;
}

void LRExpandInherit::ExtendLayer(const std::string& dir, const std::string& filepath, 
								  int layer_idx, Json::Value& dst_val)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::string full_path = ee::FileHelper::GetAbsolutePath(dir, filepath);
	std::locale::global(std::locale(""));
	std::ifstream fin(full_path.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	const Json::Value& src_layer_val = lr_val["layer"][layer_idx];
	if (!src_layer_val["base filepath"].isNull()) {
		ExtendLayer(dir, src_layer_val["base filepath"].asString(), layer_idx, dst_val);
	}

	Json::Value& dst_layer_val = dst_val["layer"][layer_idx];

	dst_layer_val["base filepath"] = "";

	int idx = 0;
	Json::Value spr_val = src_layer_val["sprite"][idx++];
	while (!spr_val.isNull()) {
		int sz = dst_layer_val["sprite"].size();
		dst_layer_val["sprite"][sz] = spr_val;
		spr_val = src_layer_val["sprite"][idx++];
	}

	idx = 0;
	Json::Value shape_val = src_layer_val["shape"][idx++];
	while (!shape_val.isNull()) {
		int sz = dst_layer_val["shape"].size();
		dst_layer_val["shape"][sz] = shape_val;
		shape_val = src_layer_val["shape"][idx++];
	}
}

}