#include "SymbolMgr.h"
#include "SymbolFactory.h"
#include "Symbol.h"
#include "Exception.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "Visitor.h"

namespace ee
{

SymbolMgr* SymbolMgr::m_instance = NULL;

SymbolMgr::SymbolMgr()
{
}

SymbolMgr::~SymbolMgr()
{
	Clear();
}

SymbolMgr* SymbolMgr::Instance()
{
	if (!m_instance)
	{
		m_instance = new SymbolMgr();
	}
	return m_instance;
}

Symbol* SymbolMgr::FetchSymbol(const std::string& filepath)
{
	std::string fixed_path = FileHelper::GetAbsolutePath(filepath);
	StringHelper::ToLower(fixed_path);

	std::map<std::string, Symbol*>::iterator itr = m_symbols.find(fixed_path);
	if (itr == m_symbols.end())
	{
		Symbol* symbol = SymbolFactory::create(fixed_path);
		if (!symbol) 
		{
			const char* path = filepath.c_str();
			throw Exception("Create symbol %s fail!", path);
//			return NULL;
		}
		bool isLoaded = symbol->LoadFromFile(fixed_path);
		if (isLoaded)
		{
			m_symbols.insert(std::make_pair(fixed_path, symbol));
			return symbol;
		}
		else
		{
			delete symbol;
// 			const char* path = filepath.c_str();
// 			throw Exception("Load symbol %s fail!", path);
			return NULL;
		}
	}
	else
	{
		itr->second->Retain();
		return itr->second;
	}
}

void SymbolMgr::Remove(const Symbol* symbol)
{
	std::string lowerpath = symbol->GetFilepath();
	StringHelper::ToLower(lowerpath);
	std::map<std::string, Symbol*>::iterator itr = m_symbols.find(lowerpath);
	// todo: new NullSymbol()
//	assert(itr != m_symbols.end());
	if (itr != m_symbols.end()) {
		m_symbols.erase(itr);
	}
}

void SymbolMgr::Clear()
{
	std::vector<Symbol*> symbols;
	symbols.reserve(m_symbols.size());
	std::map<std::string, Symbol*>::iterator itr = m_symbols.begin();
	for ( ; itr != m_symbols.end(); ++itr) {
		symbols.push_back(itr->second);
	}
	for_each(symbols.begin(), symbols.end(), ReleaseObjectFunctor<Symbol>());
	m_symbols.clear();
}

void SymbolMgr::Traverse(Visitor& visitor) const
{
	std::map<std::string, Symbol*>::const_iterator itr = m_symbols.begin();
	for ( ; itr != m_symbols.end(); ++itr)
	{
		bool next;
		visitor.Visit(itr->second, next);
		if (!next) break;
	}
}


}