#include "ScriptsSymbol.h"
#include "SymbolType.h"

#include <gum/FilepathHelper.h>

#include <fstream>

namespace ee
{

int ScriptsSymbol::Type() const
{
	return SYM_SCRIPTS;
}

bool ScriptsSymbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	fin.seekg(0, std::ios::end);   
	m_data.reserve(fin.tellg());
	fin.seekg(0, std::ios::beg);
	m_data.assign((std::istreambuf_iterator<char>(fin)),
		std::istreambuf_iterator<char>());

	std::string str = m_data;

	return true;
}

}