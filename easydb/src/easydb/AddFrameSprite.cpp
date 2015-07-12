#include "AddFrameSprite.h"
#include "check_params.h"

#include <wx/wx.h>
#include <drag2d.h>

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

void AddFrameSprite::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;
	if (!check_file(argv[3])) return;

	Trigger(argv[2], argv[3]);
}

void AddFrameSprite::Trigger(const std::string& dir, const std::string& sprite_path) const
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxString filename = files[i];
		if (!d2d::FileNameParser::isType(filename, d2d::FileNameParser::e_anim)) {
			continue;
		}

		wxString json_file = filename;

		d2d::NullSymbol symbol("");
		d2d::NullSprite sprite(&symbol);
		Json::Value actor_val;
		sprite.Store(actor_val);
		wxString dir = d2d::FilenameTools::getFileDir(json_file);
		actor_val["filepath"] = d2d::FilenameTools::getRelativePath(dir, sprite_path).ToStdString();

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(files[i].fn_str());
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
 		std::ofstream fout(json_file.fn_str());
		std::locale::global(std::locale("C"));
 		writer.write(fout, value);
 		fout.close();
	}
}

}