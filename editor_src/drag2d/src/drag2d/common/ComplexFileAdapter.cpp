#include "ComplexFileAdapter.h"
#include "FileNameTools.h"
#include "Context.h"

#include <fstream>

namespace d2d
{
	void ComplexFileAdapter::load(const char* filename)
	{
		Json::Value value;
		Json::Reader reader;
		std::ifstream fin(filename);
		reader.parse(fin, value);
		fin.close();

		m_name = value["name"].asString();

		int i = 0;
		Json::Value spriteValue = value["sprite"][i++];
		while (!spriteValue.isNull()) {
			m_data.push_back(load(spriteValue));
			spriteValue = value["sprite"][i++];
		}
	}

	ComplexFileAdapter::Entry ComplexFileAdapter::load(const Json::Value& value)
	{
		Entry entry;

		entry.filepath = d2d::FilenameTools::getAbsolutePath(
			Context::Instance()->getDir(), value["filepath"].asString());

		entry.name = value["name"].asString();
		entry.multiColor = value["multi color"].asString();
		entry.addColor = value["add color"].asString();

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
		
		entry.xMirror = value["x mirror"].asBool();
		entry.yMirror = value["y mirror"].asBool();

		return entry;
	}
}