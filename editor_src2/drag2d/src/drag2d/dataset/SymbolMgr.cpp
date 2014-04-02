#include "SymbolMgr.h"

#include "interfaces.h"

#include "SymbolFactory.h"
#include "ISymbol.h"

#include "common/Exception.h"

namespace d2d
{

SymbolMgr* SymbolMgr::m_instance = NULL;

SymbolMgr::SymbolMgr()
{
}

SymbolMgr::~SymbolMgr()
{
	clear();
}

SymbolMgr* SymbolMgr::Instance()
{
	if (!m_instance)
	{
		m_instance = new SymbolMgr();
	}
	return m_instance;
}

ISymbol* SymbolMgr::fetchSymbol(const wxString& filepath)
{
	wxString lowerpath = filepath.Lower();

	std::map<wxString, ISymbol*>::iterator itr = m_symbols.find(lowerpath);
	if (itr == m_symbols.end())
	{
		ISymbol* symbol = SymbolFactory::create(lowerpath);
		if (!symbol) 
		{
			const char* path = filepath.c_str();
			throw Exception("Create symbol %s fail!", path);
		}
		bool isLoaded = symbol->loadFromFile(lowerpath);
		if (isLoaded)
		{
			m_symbols.insert(std::make_pair(lowerpath, symbol));
			return symbol;
		}
		else
		{
			delete symbol;
			const char* path = filepath.c_str();
			throw Exception("Load symbol %s fail!", path);
			return NULL;
		}
	}
	else
	{
		itr->second->retain();
		return itr->second;
	}
}

void SymbolMgr::remove(const ISymbol* symbol)
{
	wxString lowerpath = symbol->getFilepath().Lower();
	std::map<wxString, ISymbol*>::iterator itr = m_symbols.find(lowerpath);
	assert(itr != m_symbols.end());
	if (itr != m_symbols.end()) {
		m_symbols.erase(itr);
	}
}

void SymbolMgr::clear()
{
	std::map<wxString, ISymbol*>::iterator itr = m_symbols.begin();
	for ( ; itr != m_symbols.end(); ++itr)
		delete itr->second;
	m_symbols.clear();
}

void SymbolMgr::traverse(IVisitor& visitor) const
{
	std::map<wxString, ISymbol*>::const_iterator itr = m_symbols.begin();
	for ( ; itr != m_symbols.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(itr->second, hasNext);
		if (!hasNext) break;
	}
}


} // d2d