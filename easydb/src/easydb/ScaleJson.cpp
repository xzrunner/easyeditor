#include "ScaleJson.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SpriteIO.h>

#include <fstream>

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

int ScaleJson::Run(int argc, char *argv[])
{
	// scale-json E:\test2\2000cunminnan 0.5f shadow.png

	if (!check_number(this, argc, 5)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2], atof(argv[3]), argv[4]);

	return 0;
}

void ScaleJson::Trigger(const std::string& dir, float scale, const std::string& sprite_filename) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			ScaleComplex(filepath, scale, sprite_filename);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			ScaleAnim(filepath, scale, sprite_filename);
		}
	}
}

void ScaleJson::ScaleComplex(const std::string& path, float scale, const std::string& sprite_filename) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(path);

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
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void ScaleJson::ScaleAnim(const std::string& path, float scale, const std::string& sprite_filename) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(path);

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
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void ScaleJson::ScaleSprite(Json::Value& sprite_val, float scale) const
{
	ee::SpriteIO::Data data;
	ee::SpriteIO::Load(sprite_val, data);

	data.scale *= scale;
	data.position *= scale;

	ee::SpriteIO::Store(sprite_val, data);
}

}