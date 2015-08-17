#include "SymbolSet.h"

namespace libcoco
{

void SymbolSet::Insert(const d2d::ISymbol* symbol)
{
	std::string filepath = d2d::FilenameTools::FormatFilepath(symbol->GetFilepath());

	if (filepath.find("img_guide_scale9") != std::string::npos) 
	{
		int zz  =0;
	}

	if (!Query(symbol) || d2d::FileNameParser::isType(symbol->GetFilepath(), d2d::FileNameParser::e_scale9)) {
		m_symbol_map.insert(std::make_pair(filepath, symbol));
		m_symbol_ordered.push_back(symbol);
	}
}

const d2d::ISymbol* SymbolSet::Query(const std::string& filepath) const
{
	std::multimap<std::string, const d2d::ISymbol*>::const_iterator 
		itr_begin = m_symbol_map.lower_bound(filepath),
		itr_end = m_symbol_map.upper_bound(filepath);
	if (itr_begin != itr_end) {
		return itr_begin->second;
	} else {
		return NULL;
	}
}

bool SymbolSet::Query(const d2d::ISymbol* symbol) const
{
	std::string filepath = d2d::FilenameTools::FormatFilepath(symbol->GetFilepath());
	std::multimap<std::string, const d2d::ISymbol*>::const_iterator 
		itr_begin = m_symbol_map.lower_bound(filepath),
		itr_end = m_symbol_map.upper_bound(filepath);
	std::multimap<std::string, const d2d::ISymbol*>::const_iterator itr = itr_begin;
	for ( ; itr != itr_end; ++itr) {
		if (itr->second == symbol) {
			return true;
		}
	}

	return false;
}

}