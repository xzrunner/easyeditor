#include "TexPackerAdapter.h"
#include "FileHelper.h"

#include <fstream>

namespace ee
{

void TexPackerAdapter::Load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	width = value["width"].asInt();
	height = value["height"].asInt();

	std::string dir = FileHelper::GetFileDir(filename);

	int i = 0;
	Json::Value texValue = value["image"][i++];
	while (!texValue.isNull()) {
		load(texValue, dir);
		texValue = value["image"][i++];
	}
}

void TexPackerAdapter::load(const Json::Value& value, const std::string& dir)
{
	Texture texture;

	texture.filepath = value["filepath"].asString();
	if (!FileHelper::IsFileExist(texture.filepath))
		texture.filepath = FileHelper::GetAbsolutePath(dir, texture.filepath);

	texture.region.left = value["left"].asInt();
	texture.region.low = value["low"].asInt();
	texture.region.width = value["width"].asInt();
	texture.region.height = value["height"].asInt();

	texture.bRotate = value["rotate"].asBool();

	textures.push_back(texture);
}
}