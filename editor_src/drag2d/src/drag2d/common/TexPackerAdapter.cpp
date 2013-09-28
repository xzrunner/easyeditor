#include "TexPackerAdapter.h"

#include "FileNameTools.h"

#include <fstream>

namespace d2d
{

void TexPackerAdapter::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	width = value["width"].asInt();
	height = value["height"].asInt();

	std::string dlg = FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value texValue = value["image"][i++];
	while (!texValue.isNull()) {
		load(texValue, dlg);
		texValue = value["image"][i++];
	}
}

void TexPackerAdapter::load(const Json::Value& value, const std::string& dlg)
{
	Texture texture;

	texture.filepath = value["filepath"].asString();
	if (!d2d::FilenameTools::isExist(texture.filepath))
		texture.filepath = d2d::FilenameTools::getAbsolutePath(dlg, texture.filepath);

	texture.region.left = value["left"].asInt();
	texture.region.low = value["low"].asInt();
	texture.region.width = value["width"].asInt();
	texture.region.height = value["height"].asInt();

	texture.bRotate = value["rotate"].asBool();

	textures.push_back(texture);
}
} // d2d