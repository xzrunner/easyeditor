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

bool FixSpriteColor::FixSprite(const wxString& filepath, Json::Value& sprite_val) const
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

}