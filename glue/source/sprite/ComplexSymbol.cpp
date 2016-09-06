#include "ComplexSymbol.h"
#include "FilepathHelper.h"
#include "SpriteFactory.h"

#include <json/json.h>

#include <fstream>

namespace glue
{

void ComplexSymbol::LoadJson(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_scissor.xmin = static_cast<float>(value["xmin"].asInt());
	m_scissor.xmax = static_cast<float>(value["xmax"].asInt());
	m_scissor.ymin = static_cast<float>(value["ymin"].asInt());
	m_scissor.ymax = static_cast<float>(value["ymax"].asInt());

	std::string dir = FilepathHelper::Dir(filepath);

	int i = 0;
	Json::Value spr_val = value["sprite"][i++];
	while (!spr_val.isNull()) {
		s2::Sprite* spr = SpriteFactory::Instance()->Create(spr_val, dir);
		m_children.push_back(spr);
		spr_val = value["sprite"][i++];
	}	
}

}