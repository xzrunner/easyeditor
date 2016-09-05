#include "LRExpandGroup.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SpriteIO.h>

#include <SM_Calc.h>

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

	ee::SpriteIO spr_io;
	spr_io.Load(src_spr_val);
	if (filepath == "group") 
	{
		const Json::Value& gval = src_spr_val["group"];

		if (trans.xmirror) {
			spr_io.m_position.x = -spr_io.m_position.x;
			spr_io.m_angle = -spr_io.m_angle;
		}
		if (trans.ymirror) {
			spr_io.m_position.y = -spr_io.m_position.y;
			spr_io.m_angle = -spr_io.m_angle;
		}

		Trans t;
		t.angle = spr_io.m_angle + trans.angle;
		t.scale.x = spr_io.m_scale.x * trans.scale.x;
		t.scale.y = spr_io.m_scale.y * trans.scale.y;
		t.xmirror = (spr_io.m_mirror.x && !trans.xmirror) || (!spr_io.m_mirror.x && trans.xmirror);
		t.ymirror = (spr_io.m_mirror.y && !trans.ymirror) || (!spr_io.m_mirror.y && trans.ymirror);

		float new_x = spr_io.m_position.x * trans.scale.x,
			  new_y = spr_io.m_position.y * trans.scale.y;
		t.translation = sm::rotate_vector(sm::vec2(new_x, new_y), trans.angle);
		t.translation += trans.translation;

		assert(!gval.isNull());
		for (int i = 0, n = gval.size(); i < n; ++i) {
			LoadSprites(gval[i], t, dst_sprs_val);
		}
	} 
	else 
	{
		if (trans.xmirror) {
			spr_io.m_position.x = -spr_io.m_position.x;
			spr_io.m_angle = -spr_io.m_angle;
		}
		if (trans.ymirror) {
			spr_io.m_position.y = -spr_io.m_position.y;
			spr_io.m_angle = -spr_io.m_angle;
		}

		spr_io.m_angle = spr_io.m_angle + trans.angle;
		spr_io.m_scale.x = spr_io.m_scale.x * trans.scale.x;
		spr_io.m_scale.y = spr_io.m_scale.y * trans.scale.y;
		spr_io.m_mirror.x = (spr_io.m_mirror.x && !trans.xmirror) || (!spr_io.m_mirror.x && trans.xmirror);
		spr_io.m_mirror.y = (spr_io.m_mirror.y && !trans.ymirror) || (!spr_io.m_mirror.y && trans.ymirror);
		float new_x = spr_io.m_position.x * trans.scale.x,
			  new_y = spr_io.m_position.y * trans.scale.y;
		spr_io.m_position = sm::rotate_vector(sm::vec2(new_x, new_y), trans.angle) + trans.translation;

		int sz = dst_sprs_val.size();
		dst_sprs_val[sz] = src_spr_val;
		spr_io.Store(dst_sprs_val[sz]);
	}
}

}