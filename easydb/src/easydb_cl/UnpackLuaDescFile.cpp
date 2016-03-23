#include "UnpackLuaDescFile.h"
#include "check_params.h"

#include <ee/StringHelper.h>
#include <ee/FileHelper.h>

#include <easyrespacker.h>

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

int UnpackLuaDescFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_file(argv[2])) return -1;

	Trigger(argv[2], argv[3]);

	return 0;
}

void UnpackLuaDescFile::Trigger(const std::string& lua_file, const std::string& img_name)
{
	std::vector<ee::Image*> images;
	LoadAllImages(img_name, images);

	erespacker::ResUnpacker unpacker;
	unpacker.UnpackLua(lua_file, images);
}

void UnpackLuaDescFile::LoadAllImages(const std::string& img_name, std::vector<ee::Image*>& images)
{
	int idx = 1;
	while (true)
	{
		std::string filepath = img_name + ee::StringHelper::ToString(idx++) + ".png";
		if (ee::FileHelper::IsFileExist(filepath)) {
			ee::Image* img = ee::ImageMgr::Instance()->GetItem(filepath);
			images.push_back(img);
		} else {	
			break;
		}
	}
}

}