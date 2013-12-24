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

ISymbol* SymbolMgr::getSymbol(const wxString& filepath)
{
	wxString lowerpath = filepath.Lower();

	std::map<wxString, ISymbol*>::iterator itr = m_symbols.find(lowerpath);
	if (itr == m_symbols.end())
	{
		ISymbol* symbol = SymbolFactory::create(lowerpath);
		bool isLoaded = symbol->loadFromFile(lowerpath);
		if (isLoaded)
		{
			symbol->refresh();
			m_symbols.insert(std::make_pair(lowerpath, symbol));
			return symbol;
		}
		else
		{
			delete symbol;
			throw Exception("Symbol %s load fail!", filepath.c_str());
			return NULL;
		}
	}
	else
	{
		return itr->second;
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