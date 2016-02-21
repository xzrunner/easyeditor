#include "ChangeTPJsonFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <fstream>

namespace edb
{

std::string ChangeTPJsonFile::Command() const
{
	return "change-tpjson";
}

std::string ChangeTPJsonFile::Description() const
{
	return "change TexturePacker json files' value";
}

std::string ChangeTPJsonFile::Usage() const
{
	// change-tpjson e:/test2/1001 -m 1.0 2.0

	std::string usage = Command() + " [dir path] [options] <parms>";
	usage += "-m, --move \t \"move \"frame\" xy \": [x] [y]";
	return usage;
}

void ChangeTPJsonFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return;
	if (!check_folder(argv[2])) return;

	// todo
	std::string opt = argv[3];
	if (opt != "-m" && opt != "--move") {
		std::cerr << "error opt" << std::endl;
		std::cerr << Usage() << std::endl;
		return;
	} 

	Run(argv[2]);
	if (opt == "-m" || opt == "--move") {
		TranslateFrameXY(atof(argv[4]), atof(argv[5]));
	} else {
		std::cerr << "unknown option" << std::endl;
	}
}

void ChangeTPJsonFile::Run(const std::string& dir)
{
	ee::FileHelper::FetchAllFiles(dir, m_files);
}

void ChangeTPJsonFile::TranslateFrameXY(int dx, int dy)
{
	for (int i = 0, n = m_files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(m_files[i].ToStdString());
		std::string ext = ee::FileHelper::GetExtension(filepath);
		if (ext != "json") {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		if (!check_json_key(value, "frames", filepath)) break;

 		int j = 0;
 		Json::Value itemValue = value["frames"][j++];
 		while (!itemValue.isNull()) {
			if (!check_json_key(itemValue, "frame", filepath)) break;
			if (!check_json_key(itemValue["frame"], "x", filepath)) break;
			if (!check_json_key(itemValue["frame"], "y", filepath)) break;

 			int x = itemValue["frame"]["x"].asInt();
 			int y = itemValue["frame"]["y"].asInt();
   			value["frames"][j-1]["frame"]["x"] = x + dx;
   			value["frames"][j-1]["frame"]["y"] = y + dy;
 			itemValue = value["frames"][j++];
 		}

		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));
		writer.write(fout, value);
		fout.close();
	}
}

}