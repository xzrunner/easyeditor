#include "ScaleJson.h"
#include "check_params.h"

#include <wx/filename.h>
#include <fstream>
#include <drag2d.h>

namespace edb
{

std::string ScaleJson::Command() const
{
	return "scale-json";
}

std::string ScaleJson::Description() const
{
	return "scale symbol only json file";
}

std::string ScaleJson::Usage() const
{
	return Command() + " [dir path] [scale] [sprite filename]";
}

void ScaleJson::Run(int argc, char *argv[])
{
	// scale-json E:\test2\2000cunminnan 0.5f shadow.png

	if (!check_number(this, argc, 5)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2], atof(argv[3]), argv[4]);
}

void ScaleJson::Trigger(const std::string& dir, float scale, const std::string& sprite_filename) const
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			ScaleComplex(filepath, scale, sprite_filename);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			ScaleAnim(filepath, scale, sprite_filename);
		}
	}
}

void ScaleJson::ScaleComplex(const wxString& path, float scale, const std::string& sprite_filename) const
{
	wxFileName filename(path);
	filename.Normalize();
	wxString filepath = filename.GetFullPath();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString dir = d2d::FilenameTools::getFileDir(path);

	int i = 0;
	Json::Value spriteVal = value["sprite"][i++];
	while (!spriteVal.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		if (val["filepath"].asString().find(sprite_filename) != std::string::npos) {
			ScaleSprite(val, scale);
		}
		spriteVal = value["sprite"][i++];
	}		

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.fn_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void ScaleJson::ScaleAnim(const wxString& path, float scale, const std::string& sprite_filename) const
{
	wxFileName filename(path);
	filename.Normalize();
	wxString filepath = filename.GetFullPath();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString dir = d2d::FilenameTools::getFileDir(path);

	int i = 0;
	Json::Value layerVal = value["layer"][i++];
	while (!layerVal.isNull()) {
		int j = 0;
		Json::Value frameVal = layerVal["frame"][j++];
		while (!frameVal.isNull()) {
			int k = 0;
			Json::Value entryVal = frameVal["actor"][k++];
			while (!entryVal.isNull()) {
				Json::Value& val = value["layer"][i-1]["frame"][j-1]["actor"][k-1];
				if (val["filepath"].asString().find(sprite_filename) != std::string::npos) {
					ScaleSprite(val, scale);
				}
				entryVal = frameVal["actor"][k++];
			}

			frameVal = layerVal["frame"][j++];
		}

		layerVal = value["layer"][i++];
	}		

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.fn_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void ScaleJson::ScaleSprite(Json::Value& sprite_val, float scale) const
{
	sprite_val["x scale"] = sprite_val["x scale"].asDouble() * scale;
	sprite_val["y scale"] = sprite_val["y scale"].asDouble() * scale;
	sprite_val["position"]["x"] = sprite_val["position"]["x"].asDouble() * scale;
	sprite_val["position"]["y"] = sprite_val["position"]["y"].asDouble() * scale;
}

}