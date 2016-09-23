#include "AddFrameSprite.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/DummySymbol.h>
#include <ee/DummySprite.h>

#include <fstream>

namespace edb
{

std::string AddFrameSprite::Command() const
{
	return "add-sprite";
}

std::string AddFrameSprite::Description() const
{
	return "insert sprite to each frame of anim";
}

std::string AddFrameSprite::Usage() const
{
	// add-sprite E:\new-anim\2000cunminnan\cheer E:\new-anim\shadow.png

	return Command() + " [dir path] [sprite path]";
}

int AddFrameSprite::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_file(argv[3])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void AddFrameSprite::Trigger(const std::string& dir, const std::string& sprite_path) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filename = files[i].ToStdString();
		if (!ee::FileType::IsType(filename, ee::FILE_ANIM)) {
			continue;
		}

		std::string json_file = filename;

		ee::DummySymbol sym("");
		ee::DummySprite spr(&sym);
		Json::Value actor_val;
		spr.Store(actor_val);
		std::string dir = ee::FileHelper::GetFileDir(json_file);
		actor_val["filepath"] = ee::FileHelper::GetRelativePath(dir, sprite_path);

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filename.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		int iLayer = 0;
		Json::Value layerVal = value["layer"][iLayer++];
		while (!layerVal.isNull()) {
			int iFrame = 0;
			Json::Value frameVal = layerVal["frame"][iFrame++];
			while (!frameVal.isNull()) {
				Json::Value lastVal = actor_val;
				int iActor = 0;
				Json::Value entryVal = frameVal["actor"][iActor++];
				while (!entryVal.isNull()) {
					value["layer"][iLayer-1]["frame"][iFrame-1]["actor"][iActor-1] = lastVal;
					lastVal = entryVal;
					entryVal = frameVal["actor"][iActor++];
				}
				value["layer"][iLayer-1]["frame"][iFrame-1]["actor"][iActor-1] = lastVal;				
				frameVal = layerVal["frame"][iFrame++];
			}
			layerVal = value["layer"][iLayer++];
		}

 		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
 		std::ofstream fout(json_file.c_str());
		std::locale::global(std::locale("C"));
 		writer.write(fout, value);
 		fout.close();
	}
}

}