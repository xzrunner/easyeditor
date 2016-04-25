#include "LRExpandGroup.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/Vector.h>
#include <ee/Math2D.h>

#include <fstream>

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

int LRExpandGroup::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_file(argv[2])) return -1;

	Run(argv[2]);

	return 0;
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

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	Json::Value new_lr_val = lr_val;

	int idx = 0;
	Json::Value layer_val = lr_val["layer"][idx++];
	while (!layer_val.isNull()) {
		Expand(layer_val, new_lr_val["layer"][idx-1]);
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

std::string LRExpandGroup::GetOutputFilepath(const std::string& filepath) const
{
	std::string out_path = filepath;
	out_path.insert(filepath.find("_lr.json"), "_group");
	return out_path;
}

void LRExpandGroup::Expand(const Json::Value& src_val, Json::Value& dst_val)
{
	Json::Value sprites_val;

	int idx = 0;
	Json::Value spr_val = src_val["sprite"][idx++];
	while (!spr_val.isNull()) {
		LoadSprites(spr_val, Trans(), sprites_val);
		spr_val = src_val["sprite"][idx++];
	}

	dst_val["sprite"] = sprites_val;
}

void LRExpandGroup::LoadSprites(const Json::Value& src_spr_val, const Trans& trans, Json::Value& dst_sprs_val)
{
	std::string filepath = src_spr_val["filepath"].asString();
	if (filepath == "group") 
	{
		const Json::Value& gval = src_spr_val["group"];

		ee::Vector translation;
		translation.x = src_spr_val["position"]["x"].asDouble();
		translation.y = src_spr_val["position"]["y"].asDouble();
		float angle = src_spr_val["angle"].asDouble();
		ee::Vector scale;
		scale.x = src_spr_val["x scale"].asDouble();
		scale.y = src_spr_val["y scale"].asDouble();
		bool xmirror = src_spr_val["x mirror"].asBool(),
			 ymirror = src_spr_val["y mirror"].asBool();

		Trans t;
		t.angle = angle + trans.angle;
		t.scale.x = scale.x * trans.scale.x;
		t.scale.y = scale.y * trans.scale.y;
		t.xmirror = (xmirror && !trans.xmirror) || (!xmirror && trans.xmirror);
		t.ymirror = (ymirror && !trans.ymirror) || (!ymirror && trans.ymirror);

		float new_x = translation.x * trans.scale.x,
			  new_y = translation.y * trans.scale.y;
		t.translation = ee::Math2D::RotateVector(ee::Vector(new_x, new_y), trans.angle);
		t.translation += trans.translation;

		assert(!gval.isNull());
		for (int i = 0, n = gval.size(); i < n; ++i) {
			LoadSprites(gval[i], t, dst_sprs_val);
		}
	} 
	else 
	{
		float old_x = src_spr_val["position"]["x"].asDouble(),
			  old_y = src_spr_val["position"]["y"].asDouble();
		float old_angle = src_spr_val["angle"].asDouble();
		float old_sx = src_spr_val["x scale"].asDouble(),
			  old_sy = src_spr_val["y scale"].asDouble();
		bool old_mx = src_spr_val["x mirror"].asBool(),
			 old_my = src_spr_val["y mirror"].asBool();

		int sz = dst_sprs_val.size();
		dst_sprs_val[sz] = src_spr_val;
		dst_sprs_val[sz]["angle"] = old_angle + trans.angle;
		dst_sprs_val[sz]["x scale"] = old_sx * trans.scale.x;
		dst_sprs_val[sz]["y scale"] = old_sy * trans.scale.y;
		dst_sprs_val[sz]["x mirror"] = (old_mx && !trans.xmirror) || (!old_mx && trans.xmirror);
		dst_sprs_val[sz]["y mirror"] = (old_my && !trans.ymirror) || (!old_my && trans.ymirror);

		float new_x = old_x * trans.scale.x,
			  new_y = old_y * trans.scale.y;
		ee::Vector new_pos = ee::Math2D::RotateVector(ee::Vector(new_x, new_y), trans.angle);
		new_pos += trans.translation;

		dst_sprs_val[sz]["position"]["x"] = new_pos.x;
		dst_sprs_val[sz]["position"]["y"] = new_pos.y;
	}
}

}