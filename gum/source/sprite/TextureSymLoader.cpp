#include "TextureSymLoader.h"
#include "FilepathHelper.h"
#include "ShapeLoader.h"

#include <sprite2/TextureSymbol.h>
#include <sprite2/PolygonShape.h>

#include <fstream>

namespace gum
{

TextureSymLoader::TextureSymLoader(s2::TextureSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

TextureSymLoader::~TextureSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void TextureSymLoader::LoadJson(const std::string& filepath)
{
	if (!m_sym) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (!value.isMember("shapes")) {
		return;
	}

	std::string dir = FilepathHelper::Dir(filepath);

	for (int i = 0, n = value["shapes"].size(); i < n; ++i) {
		s2::Shape* shape = ShapeLoader::LoadShape(value["shapes"][i], dir);
		m_sym->AddPolygon(VI_DOWNCASTING<s2::PolygonShape*>(shape));
	}
}

}