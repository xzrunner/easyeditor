#include "LRToShapeTable.h"
#include "check_params.h"

namespace edb
{

std::string LRToShapeTable::Command() const
{
	return "lr2shape";
}

std::string LRToShapeTable::Description() const
{
	return "create shape table from lr file";
}

std::string LRToShapeTable::Usage() const
{
	// lr2shape e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

void LRToShapeTable::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2]);
}

void LRToShapeTable::Run(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	Json::Value complex_val;
	for (int i = 0; i < 3; ++i) {
		LoadSpriteValue(lr_val["layer"][i]["sprite"], complex_val["sprite"]);
	}
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

}