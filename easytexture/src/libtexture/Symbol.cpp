#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/Shape.h>

#include <easyshape.h>

#include <sprite2/PolygonShape.h>

#include <json/json.h>

#include <fstream>

namespace etexture
{

Symbol::Symbol()
{
}

void Symbol::LoadResources()
{
	Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);

	int i = 0;
	Json::Value shape_val = value["shapes"][i++];
	while (!shape_val.isNull()) {
		ee::Shape* shape = eshape::FileIO::LoadShape(dir, shape_val);
		m_polygons.push_back(dynamic_cast<s2::PolygonShape*>(shape));
		shape_val = value["shapes"][i++];
	}
}

}