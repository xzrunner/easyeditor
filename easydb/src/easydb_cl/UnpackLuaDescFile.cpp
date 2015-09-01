#include "UnpackLuaDescFile.h"
#include "check_params.h"

#include <drag2d.h>
#include <easycoco.h>

namespace edb
{

std::string UnpackLuaDescFile::Command() const
{
	return "unpack-lua-desc";
}

std::string UnpackLuaDescFile::Description() const
{
	return "unpack lua desc file";
}

std::string UnpackLuaDescFile::Usage() const
{
	return Command() + " [lua file] [img name]";
}

void UnpackLuaDescFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Trigger(argv[2], argv[3]);
}

void UnpackLuaDescFile::Trigger(const std::string& lua_file, const std::string& img_name)
{
	std::vector<d2d::Image*> images;
	LoadAllImages(img_name, images);

	libcoco::CocoUnpacker unpacker(lua_file, images);
}

void UnpackLuaDescFile::LoadAllImages(const std::string& img_name, std::vector<d2d::Image*>& images)
{
	int idx = 1;
	while (true)
	{
		std::string filepath = img_name + d2d::StringTools::IntToString(idx++) + ".png";
		if (d2d::FilenameTools::IsFileExist(filepath)) {
			d2d::Image* img = d2d::ImageMgr::Instance()->GetItem(filepath);
			images.push_back(img);
		} else {	
			break;
		}
	}
}

}