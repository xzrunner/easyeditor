#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/Shape.h>

#include <easyshape.h>

#include <sprite2/PolygonShape.h>
#include <gum/FilepathHelper.h>

#include <json/json.h>

#include <fstream>

namespace etexture
{

Symbol::Symbol()
{
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

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
		auto shape = eshape::FileIO::LoadShape(dir, shape_val);
		m_polygons.push_back(std::dynamic_pointer_cast<s2::PolygonShape>(shape));
		shape_val = value["shapes"][i++];
	}

	return true;
}

}