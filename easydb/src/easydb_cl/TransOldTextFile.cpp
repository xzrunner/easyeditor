#include "TransOldTextFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <wx/arrstr.h>

#include <fstream>

namespace edb
{

std::string TransOldTextFile::Command() const
{
	return "trans-old-text";
}

std::string TransOldTextFile::Description() const
{
	return "trans old text file";
}

std::string TransOldTextFile::Usage() const
{
	// trans-old-text dir
	std::string usage = Command() + " [folder]";
	return usage;
}

void TransOldTextFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Run(argv[2]);
}

void TransOldTextFile::Run(const std::string& folder)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(folder, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			TransComplex(filepath);
		}
	}
}

void TransOldTextFile::TransComplex(const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	bool dirty = false;
	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		Json::Value& val = value["sprite"][i];
		if (TransSprite(val)) {
			dirty = true;
		}
	}
	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

bool TransOldTextFile::TransSprite(Json::Value& value) const
{
	std::string filepath = value["filepath"].asString();
	if (filepath.find("_fontblank.json") == std::string::npos) {
		return false;
	}

	value["filepath"] = filepath.substr(0, filepath.find("_fontblank.json")) + "_text.json";
	
	Json::Value txt_val;
	txt_val["align_hori"] = 2;
	txt_val["align_vert"] = 2;
	txt_val["edge"] = true;
	txt_val["edge_color"] = "0x000000ff";
	txt_val["edge_size"] = 1.0f;
	txt_val["font"] = 0;
	txt_val["font_color"] = "0xffffffff";
	txt_val["font_size"] = 25;
	txt_val["space_hori"] = 1.0f;
	txt_val["space_vert"] = 1.0f;
	txt_val["width"] = 100;
	txt_val["height"] = 100;
	txt_val["text"] = "";
	txt_val["tid"] = "";

	if (!value["align hori"].isNull()) {
		txt_val["align_hori"] = value["align hori"];
		txt_val["align_vert"] = value["align vert"];

		Json::Value test = value.removeMember("align hori");
		value.removeMember("align vert");
	}
	if (!value["font"].isNull()) {
		std::string font = value["font"].asString();
		txt_val["edge"] = font == "E";
		value["font"] = NULL;
	}
	if (!value["color"].isNull()) {
		txt_val["font_color"] = value["color"];
		value["color"] = NULL;
	}
	if (!value["size"].isNull()) {
		txt_val["font_size"] = value["size"];
	}
	if (!value["label_width"].isNull()) {
		txt_val["width"] = value["label_width"];
		txt_val["height"] = value["label_height"];
		value["label_width"] = NULL;
		value["label_height"] = NULL;
	}
	if (!value["content"].isNull()) {
		txt_val["text"] = value["content"];
		txt_val["tid"] = value["tid"];
		value["content"] = NULL;
		value["tid"] = NULL;
	}

	value["text"] = txt_val;

	return true;
}

}