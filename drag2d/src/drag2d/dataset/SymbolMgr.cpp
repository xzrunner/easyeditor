#include "SymbolMgr.h"

#include "interfaces.h"

#include "SymbolFactory.h"
#include "ISymbol.h"

#include "common/Exception.h"
#include "common/FileNameTools.h"

#include <wx/filename.h>

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
	wxString fixedPath = FilenameTools::getExistFilepath(filepath.Lower());
	wxFileName filename(fixedPath);
	filename.Normalize();
	fixedPath = filename.GetFullPath().Lower();

	std::map<wxString, ISymbol*>::iterator itr = m_symbols.find(fixedPath);
	if (itr == m_symbols.end())
	{
		ISymbol* symbol = SymbolFactory::create(fixedPath);
		if (!symbol) 
		{
// 			const char* path = filepath.c_str();
// 			throw Exception("Create symbol %s fail!", path);
			return NULL;
		}
		bool isLoaded = symbol->loadFromFile(fixedPath);
		if (isLoaded)
		{
			m_symbols.insert(std::make_pair(fixedPath, symbol));
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

void SymbolMgr::remove(const ISymbol* symbol)
{
	wxString lowerpath = symbol->getFilepath().Lower();
	std::map<wxString, ISymbol*>::iterator itr = m_symbols.find(lowerpath);
	// todo: new NullSymbol()
//	assert(itr != m_symbols.end());
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