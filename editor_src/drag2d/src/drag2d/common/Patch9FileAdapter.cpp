#include "Patch9FileAdapter.h"
#include "FileNameTools.h"
#include "Context.h"

#include <fstream>

namespace d2d
{
	void Patch9FileAdapter::load(const char* filename)
	{
		Json::Value value;
		Json::Reader reader;
		std::ifstream fin(filename);
		reader.parse(fin, value);
		fin.close();

		type = value["type"].asInt();

		width = value["width"].asInt();
		height = value["height"].asInt();

		int i = 0;
		Json::Value spriteValue = value["sprite"][i++];
		while (!spriteValue.isNull()) {
			m_data.push_back(load(spriteValue));
			spriteValue = value["sprite"][i++];
		}
	}

	Patch9FileAdapter::Entry Patch9FileAdapter::load(const Json::Value& value)
	{
		Entry entry;

		entry.filepath = d2d::FilenameTools::getAbsolutePath(
			Context::Instance()->getDir(), value["filepath"].asString());

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