#include "SymbolSet.h"

#include <ee/FileHelper.h>
#include <ee/Symbol.h>

namespace ecoco
{

void SymbolSet::Insert(const ee::Symbol* symbol)
{
	std::string filepath = ee::FileHelper::FormatFilepath(symbol->GetFilepath());

	if (filepath.find("img_guide_scale9") != std::string::npos) 
	{
		int zz  =0;
	}

	if (!Query(symbol) || ee::FileType::IsType(symbol->GetFilepath(), ee::FileType::e_scale9)) {
		m_symbol_map.insert(std::make_pair(filepath, symbol));
		m_symbol_ordered.push_back(symbol);
	}
}

const ee::Symbol* SymbolSet::Query(const std::string& filepath) const
{
	std::multimap<std::string, const ee::Symbol*>::const_iterator 
		itr_begin = m_symbol_map.lower_bound(filepath),
		itr_end = m_symbol_map.upper_bound(filepath);
	if (itr_begin != itr_end) {
		return itr_begin->second;
	} else {
		return NULL;
	}
}

bool SymbolSet::Query(const ee::Symbol* symbol) const
{
	std::string filepath = ee::FileHelper::FormatFilepath(symbol->GetFilepath());
	std::multimap<std::string, const ee::Symbol*>::const_iterator 
		itr_begin = m_symbol_map.lower_bound(filepath),
		itr_end = m_symbol_map.upper_bound(filepath);
	std::multimap<std::string, const ee::Symbol*>::const_iterator itr = itr_begin;
	for ( ; itr != itr_end; ++itr) {
		if (itr->second == symbol) {
			return true;
		}
	}

	return false;
}

}