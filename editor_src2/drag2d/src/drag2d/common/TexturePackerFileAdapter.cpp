#include "TexturePackerFileAdapter.h"

#include <fstream>

#include <drag2d.h>

namespace d2d
{

void TexturePackerFileAdapter::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	width = value["meta"]["size"]["w"].asInt();
	height = value["meta"]["size"]["h"].asInt();

	int i = 0;
	Json::Value frameValue = value["frames"][i++];
	while (!frameValue.isNull()) {
		Entry entry;
		load(frameValue, entry);
		frames.push_back(entry);
		frameValue = value["frames"][i++];
	}
}

void TexturePackerFileAdapter::load(const Json::Value& value, Entry& entry)
{
	entry.filename = value["filename"].asString();
	StringTools::toLower(entry.filename);
	load(value["frame"], entry.frame);
	entry.rotated = value["rotated"].asBool();
	entry.trimmed = value["trimmed"].asBool();
	load(value["spriteSourceSize"], entry.spriteSourceSize);
	entry.srcWidth = value["sourceSize"]["w"].asInt();
	entry.srcHeight = value["sourceSize"]["h"].asInt();
}

void TexturePackerFileAdapter::load(const Json::Value& value, Region& region)
{
	region.x = value["x"].asInt();
	region.y = value["y"].asInt();
	region.w = value["w"].asInt();
	region.h = value["h"].asInt();
}
} // d2d