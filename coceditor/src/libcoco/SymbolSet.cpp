#include "SymbolSet.h"

namespace libcoco
{

void SymbolSet::Insert(const d2d::ISymbol* symbol)
{
	std::string filepath = d2d::FilenameTools::FormatFilepath(symbol->GetFilepath());
	std::map<std::string, const d2d::ISymbol*>::const_iterator itr = 
		m_symbol_map.find(filepath);
	if (itr == m_symbol_map.end()) {
		m_symbol_map.insert(std::make_pair(filepath, symbol));
		m_symbol_ordered.push_back(symbol);
	}
}

const d2d::ISymbol* SymbolSet::Query(const std::string& filepath) const
{
	std::map<std::string, const d2d::ISymbol*>::const_iterator itr = 
		m_symbol_map.find(filepath);
	if (itr != m_symbol_map.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

bool SymbolSet::Query(const d2d::ISymbol* symbol) const
{
	std::string filepath = d2d::FilenameTools::FormatFilepath(symbol->GetFilepath());
	std::map<std::string, const d2d::ISymbol*>::const_iterator itr = 
		m_symbol_map.find(filepath);
	return itr != m_symbol_map.end();
}

}