#include "LRExpandGroup.h"
#include "check_params.h"

namespace edb
{

std::string LRExpandGroup::Command() const
{
	return "lr-expand-group";
}

std::string LRExpandGroup::Description() const
{
	return "expand group";
}

std::string LRExpandGroup::Usage() const
{
	// lr-expand-group e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

void LRExpandGroup::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2]);
}

void LRExpandGroup::Run(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	std::string dir = d2d::FileHelper::GetFileDir(filepath);

	Json::Value new_lr_val = lr_val;

	int idx = 0;
	Json::Value layer_val = lr_val["layer"][idx++];
	while (!layer_val.isNull()) {
		Expand(layer_val, new_lr_val["layer"][idx-1]);
		layer_val = lr_val["layer"][idx++];
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, new_lr_val);
	fout.close();
}

void LRExpandGroup::Expand(const Json::Value& src_val, Json::Value& dst_val)
{
	Json::Value sprites_val;

	int idx = 0;
	Json::Value spr_val = src_val["sprite"][idx++];
	while (!spr_val.isNull()) {
		LoadSprites(spr_val, sprites_val);
		spr_val = src_val["sprite"][idx++];
	}

	dst_val["sprite"] = sprites_val;
}

void LRExpandGroup::LoadSprites(const Json::Value& src_spr_val, Json::Value& dst_sprs_val)
{
	std::string filepath = src_spr_val["filepath"].asString();
	if (filepath == "group") {
		const Json::Value& gval = src_spr_val["group"];
		assert(!gval.isNull());
		for (int i = 0, n = gval.size(); i < n; ++i) {
			LoadSprites(gval[i], dst_sprs_val);
		}
	} else {
		int sz = dst_sprs_val.size();
		dst_sprs_val[sz] = src_spr_val;
	}
}

}