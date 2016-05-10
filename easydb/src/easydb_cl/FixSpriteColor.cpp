#include "FixSpriteColor.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/trans_color.h>

#include <sprite2/Color.h>

#include <fstream>

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

int FixSpriteColor::Run(int argc, char *argv[])
{
	// fix-color D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

bool FixSpriteColor::FixSprite(const std::string& filepath, Json::Value& sprite_val) const
{
	if (sprite_val["add color"].isNull()) {
		return false;
	}
	std::string str = sprite_val["add color"].asString();
	if (str.compare("0x00000000") == 0) {
		return false;
	}

	s2::Color col = ee::str2color(str, ee::PT_ARGB);
	sprite_val["add color"] = ee::color2str(col, ee::PT_ABGR);

	return true;
}

void FixSpriteColor::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			FixComplex(filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			FixAnim(filepath);
		}
	}
}

}