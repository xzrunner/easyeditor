#include "LRToComplex.h"
#include "check_params.h"

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

void LRToComplex::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2]);
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

	std::set<std::string> invisible;
	InitInvisibleSet(lr_val["layer"]["collision region"], invisible);
	InitInvisibleSet(lr_val["layer"]["point"], invisible);
	InitInvisibleSet(lr_val["layer"]["path"], invisible);
	InitInvisibleSet(lr_val["layer"]["region"], invisible);

	Json::Value complex_val;
	complex_val["sprite"] = FilterSprites(lr_val["sprite"], invisible);
	complex_val["name"] = "";
	complex_val["use_render_cache"] = false;
	complex_val["xmax"] = 0;
	complex_val["xmin"] = 0;
	complex_val["ymax"] = 0;
	complex_val["ymin"] = 0;

	std::string outfile = filepath.substr(0, filepath.find_last_of("_"));
	wxString tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_complex);
	outfile = d2d::FilenameTools::getFilenameAddTag(outfile, tag, "json");

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, complex_val);
	fout.close();
}

void LRToComplex::InitInvisibleSet(const Json::Value& layer_val, 
								   std::set<std::string>& invisible) const
{
	const Json::Value& symbols_val = layer_val["symbol"];

	int idx = 0;
	Json::Value symbol_val = symbols_val[idx++];
	while (!symbol_val.isNull()) {
		invisible.insert(symbol_val.asString());
		symbol_val = symbols_val[idx++];
	}
}

Json::Value LRToComplex::FilterSprites(const Json::Value& sprite_val, 
									   const std::set<std::string>& invisible) const
{
	Json::Value result;
	int r_idx = 0;

	int idx = 0;
	Json::Value spr_val = sprite_val[idx++];
	while (!spr_val.isNull()) {
		std::string filepath = spr_val["filepath"].asString();
		if (invisible.find(filepath) == invisible.end()) {
			Json::Value spr_val_fix = spr_val;

			std::string suffix = "_shape.json";
			int pos = filepath.find(suffix);
 			if (pos!= std::string::npos) {
				std::string fix_filepath = filepath.substr(0, pos) + ".png";
				if (d2d::FilenameTools::isExist(fix_filepath)) {
					spr_val_fix["filepath"] = fix_filepath;
				}
 			}

			result[r_idx++] = spr_val_fix;
		}
		spr_val = sprite_val[idx++];
	}

	return result;
}

}