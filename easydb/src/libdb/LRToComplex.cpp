#include "LRToComplex.h"
#include "check_params.h"
#include "lr_typedef.h"
#include "lr_tools.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <fstream>

namespace edb
{

std::string LRToComplex::Command() const
{
	return "lr2complex";
}

std::string LRToComplex::Description() const
{
	return "create complex file from lr file";
}

std::string LRToComplex::Usage() const
{
	// lr2complex e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

int LRToComplex::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_file(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void LRToComplex::Run(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	SeparateBottomLayer(lr_val, filepath);
	SeparateTopLayer(lr_val, filepath);
}

void LRToComplex::SeparateBottomLayer(const Json::Value& lr_val, const std::string& filepath)
{
	Json::Value complex_val;
	for (int layer_idx = 0; layer_idx < 2; ++layer_idx) 
	{
		const Json::Value& layer_val = lr_val["layer"][layer_idx];

		LoadAllSpriteValue(layer_val["sprite"], complex_val["sprite"], true);

		int idx = 0;
		Json::Value cl_val = layer_val["layers"][idx++];
		while (!cl_val.isNull()) {
			LoadAllSpriteValue(cl_val, complex_val["sprite"], true);
			cl_val = layer_val["layers"][idx++];
		}
	}
	OutputComplexFile(complex_val, filepath, "base");
}

void LRToComplex::SeparateTopLayer(const Json::Value& lr_val, const std::string& filepath)
{
	Json::Value complex_val;
	for (int layer_idx = 0; layer_idx < 2; ++layer_idx) 
	{
		const Json::Value& layer_val = lr_val["layer"][layer_idx];

		LoadAllSpriteValue(layer_val["sprite"], complex_val["sprite"], false);

		int idx = 0;
		Json::Value cl_val = layer_val["layers"][idx++];
		while (!cl_val.isNull()) {
			LoadAllSpriteValue(cl_val, complex_val["sprite"], false);
			cl_val = layer_val["layers"][idx++];
		}
	}
	OutputComplexFile(complex_val, filepath, "top");
}

void LRToComplex::LoadAllSpriteValue(const Json::Value& src_val, Json::Value& dst_val, bool bottom_layer)
{
	int idx = 0;
	Json::Value spr_val = src_val[idx++];
	while (!spr_val.isNull()) {
		std::string tag = spr_val["tag"].asString();
		if (bottom_layer) {
			// rm all sprite with tag info
			if (!tag.empty()) {
				spr_val = src_val[idx++];
				continue;
			}
		} else {
			if (tag.find(TOP_LAYER_STR) == std::string::npos) {
				spr_val = src_val[idx++];
				continue;
			}
		}

		LoadSpriteValue(spr_val, dst_val);

		spr_val = src_val[idx++];
	}
}

void LRToComplex::LoadSpriteValue(const Json::Value& spr_val, Json::Value& dst_val)
{
	std::string filepath = spr_val["filepath"].asString();
	Json::Value spr_val_fix = spr_val;
	std::string suffix = "_shape.json";
	int pos = filepath.find(suffix);
	if (pos!= std::string::npos) {
		std::string fix_filepath = filepath.substr(0, pos) + ".png";
		if (ee::FileHelper::IsFileExist(fix_filepath)) {
			spr_val_fix["filepath"] = fix_filepath;
		}
	}	
	dst_val[dst_val.size()] = spr_val_fix;
}

void LRToComplex::OutputComplexFile(Json::Value& complex_val, const std::string& filepath,
									const std::string& tag)
{
	std::string name = get_lr_name_from_file(filepath) + "_" + tag;
	complex_val["name"] = name;
	complex_val["use_render_cache"] = false;
	complex_val["xmax"] = 0;
	complex_val["xmin"] = 0;
	complex_val["ymax"] = 0;
	complex_val["ymin"] = 0;

	std::string outfile = ee::FileHelper::GetFileDir(filepath) + "\\" + name 
		+ "_" + ee::SymbolFile::Instance()->Tag(s2::SYM_COMPLEX) + ".json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, complex_val);
	fout.close();
}

}