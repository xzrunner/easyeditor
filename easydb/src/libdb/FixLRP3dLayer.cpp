#include "FixLRP3dLayer.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <SM_Vector.h>
#include <sprite2/Color.h>
#include <gum/trans_color.h>

#include <fstream>

namespace edb
{

std::string FixLRP3dLayer::Command() const
{
	return "fix-lr-p3d-layer";
}

std::string FixLRP3dLayer::Description() const
{
	return "fix LR P3d layer";
}

std::string FixLRP3dLayer::Usage() const
{
	return Command() + " [dir]";
}

int FixLRP3dLayer::Run(int argc, char *argv[])
{
	// fix-lr-p3d-layer D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

bool FixLRP3dLayer::FixSprite(Json::Value& val) const
{
	std::string filepath = val["filepath"].asString();
	if (filepath.find("_particle.json") == std::string::npos) {
		return false;
	}
	if (val.isMember("tag")) {
		return false;
	}

	val["tag"] = "layer=top;";

	return true;
}

void FixLRP3dLayer::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (filepath.find("_lr.json") == std::string::npos) {
			continue;
		}

		Json::Value val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, val);
		fin.close();

		bool dirty = false;
		for (int i = 0, n = val["layer"].size(); i < n; ++i) {
			Json::Value& layer_val = val["layer"][i];
			for (int j = 0, m = layer_val["sprite"].size(); j < m; ++j) {
				if (FixSprite(layer_val["sprite"][j])) {
					dirty = true;
				}
			}
		}

		if (dirty) {
			Json::StyledStreamWriter writer;
			std::locale::global(std::locale(""));
			std::ofstream fout(filepath.c_str());
			std::locale::global(std::locale("C"));	
			writer.write(fout, val);
			fout.close();
		}
	}
}

}