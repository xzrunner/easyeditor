#include "ShapeSymLoader.h"

#include <sprite2/ShapeSymbol.h>

#include <fstream>

namespace gum
{

ShapeSymLoader::ShapeSymLoader(s2::ShapeSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

ShapeSymLoader::~ShapeSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void ShapeSymLoader::LoadJson(const std::string& filepath)
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

	// todo
}

}