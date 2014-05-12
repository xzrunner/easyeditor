#include "FileLoader.h"

namespace escale9
{

void FileLoader::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	type = value["type"].asInt();

	width = value["width"].asInt();
	height = value["height"].asInt();

	wxString dir = d2d::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		m_data.push_back(load(spriteValue, dir));
		spriteValue = value["sprite"][i++];
	}
}

FileLoader::Entry FileLoader::load(const Json::Value& value, const wxString& dir)
{
	Entry entry;

	entry.filepath = d2d::FilenameTools::getAbsolutePath(
		dir, value["filepath"].asString());

	entry.name = value["name"].asString();

	entry.pos.x = value["position"]["x"].asDouble();
	entry.pos.y = value["position"]["y"].asDouble();

	entry.angle = value["angle"].asDouble();

	if (value["scale"].isNull())
	{
		entry.xscale = value["x scale"].asDouble();
		entry.yscale = value["y scale"].asDouble();
	}
	else
	{
		entry.xscale = entry.yscale = value["scale"].asDouble();
	}

	if (!value["x shear"].isNull())
	{
		entry.xshear = value["x shear"].asDouble();
		entry.yshear = value["y shear"].asDouble();
	}
	else
	{
		entry.xshear = entry.yshear = 0;
	}

	entry.xMirror = value["x mirror"].asDouble();
	entry.yMirror = value["y mirror"].asDouble();

	return entry;
}

}