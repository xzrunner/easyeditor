#include "SymbolMgr.h"

#include "interfaces.h"

#include "SymbolFactory.h"
#include "ISymbol.h"

#include "common/Exception.h"
#include "common/FileNameTools.h"
#include "common/StringTools.h"

#include <wx/filename.h>

namespace d2d
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

ISymbol* SymbolMgr::FetchSymbol(const std::string& filepath)
{
	std::string fixed_path = filepath;
	StringTools::ToLower(fixed_path);
	fixed_path = FilenameTools::getExistFilepath(fixed_path);
	wxFileName filename(fixed_path);
	filename.Normalize();
	fixed_path = filename.GetFullPath().Lower();

	std::map<std::string, ISymbol*>::iterator itr = m_symbols.find(fixed_path);
	if (itr == m_symbols.end())
	{
		ISymbol* symbol = SymbolFactory::create(fixed_path);
		if (!symbol) 
		{
// 			const char* path = filepath.c_str();
// 			throw Exception("Create symbol %s fail!", path);
			return NULL;
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

void SymbolMgr::Remove(const ISymbol* symbol)
{
	std::string lowerpath = symbol->GetFilepath();
	StringTools::ToLower(lowerpath);
	std::map<std::string, ISymbol*>::iterator itr = m_symbols.find(lowerpath);
	// todo: new NullSymbol()
//	assert(itr != m_symbols.end());
	if (itr != m_symbols.end()) {
		m_symbols.erase(itr);
	}
}

void SymbolMgr::Clear()
{
	std::map<std::string, ISymbol*>::iterator itr = m_symbols.begin();
	for ( ; itr != m_symbols.end(); ++itr)
		delete itr->second;
	m_symbols.clear();
}

void SymbolMgr::Traverse(IVisitor& visitor) const
{
	std::map<std::string, ISymbol*>::const_iterator itr = m_symbols.begin();
	for ( ; itr != m_symbols.end(); ++itr)
	{
		bool hasNext;
		visitor.Visit(itr->second, hasNext);
		if (!hasNext) break;
	}
}


} // d2d