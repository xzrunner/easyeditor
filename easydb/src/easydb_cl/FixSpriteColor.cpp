#include "FixSpriteColor.h"
#include "check_params.h"

#include <wx/filename.h>
#include <fstream>
#include <drag2d.h>

namespace edb
{

std::string FixSpriteColor::Command() const
{
	return "fix-color";
}

std::string FixSpriteColor::Description() const
{
	return "fix sprite's add color, change red and blue";
}

std::string FixSpriteColor::Usage() const
{
	return Command() + " [dir]";
}

void FixSpriteColor::Run(int argc, char *argv[])
{
	// fix-color D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void FixSpriteColor::Trigger(const std::string& dir) const
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			FixComplex(filepath);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			FixAnim(filepath);
		}
	}
}

void FixSpriteColor::FixComplex(const wxString& path) const
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

	bool dirty = false;

	int i = 0;
	Json::Value spriteVal = value["sprite"][i++];
	while (!spriteVal.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		if (FixSprite(val)) {
			dirty = true;
		}
		spriteVal = value["sprite"][i++];
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.fn_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

void FixSpriteColor::FixAnim(const wxString& path) const
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

	bool dirty = false;

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
				if (FixSprite(val)) {
					dirty = true;
				}
				entryVal = frameVal["actor"][k++];
			}

			frameVal = layerVal["frame"][j++];
		}

		layerVal = value["layer"][i++];
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.fn_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

bool FixSpriteColor::FixSprite(Json::Value& sprite_val) const
{
	if (sprite_val["add color"].isNull()) {
		return false;
	}
	std::string str = sprite_val["add color"].asString();
	if (str.compare("0x00000000") == 0) {
		return false;
	}

	d2d::Colorf col = d2d::transColor(str, d2d::PT_ARGB);
	sprite_val["add color"] = d2d::transColor(col, d2d::PT_ABGR);

	return true;
}

}