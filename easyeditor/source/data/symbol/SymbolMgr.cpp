#include "SymbolMgr.h"
#include "SymbolFactory.h"
#include "Symbol.h"
#include "Exception.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "Visitor.h"
#include "SymbolType.h"

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

Symbol* SymbolMgr::FetchSymbol(const std::string& filepath, int type)
{
	std::string fixed_path = FileHelper::GetAbsolutePath(filepath);
	StringHelper::ToLower(fixed_path);

	std::string filename = FileHelper::GetFilename(filepath);
	std::map<std::string, Symbol*>::iterator itr = m_syms.find(fixed_path);
	if (filename == SYM_GROUP_TAG || itr == m_syms.end())
	{
		Symbol* sym = SymbolFactory::Create(fixed_path, type);
		if (!sym) 
		{
			const char* path = filepath.c_str();
			throw Exception("Create symbol fail: %s", path);
//			return NULL;
		}

		bool succ = sym->LoadFromFile(fixed_path);
		if (succ)
		{
			sym->AddReference();
			m_syms.insert(std::make_pair(fixed_path, sym));
			return sym;
		}
		else
		{
// 			sym->RemoveReference();
// 			const char* path = filepath.c_str();
// 			throw Exception("Load symbol %s fail!", path);

			return NULL;
		}
	}
	else
	{
		itr->second->AddReference();
		return itr->second;
	}
}

void SymbolMgr::Remove(const Symbol* sym)
{
	std::string lowerpath = sym->GetFilepath();
	StringHelper::ToLower(lowerpath);
	std::map<std::string, Symbol*>::iterator itr = m_syms.find(lowerpath);
	// todo: new DummySymbol()
//	assert(itr != m_syms.end());
	if (itr != m_syms.end()) {
		m_syms.erase(itr);
	}
}

void SymbolMgr::Clear()
{
	std::vector<Symbol*> syms;
	syms.reserve(m_syms.size());
	std::map<std::string, Symbol*>::iterator itr = m_syms.begin();
	for ( ; itr != m_syms.end(); ++itr) {
		syms.push_back(itr->second);
	}
	for_each(syms.begin(), syms.end(), cu::RemoveRefFunctor<Symbol>());
	m_syms.clear();
}

void SymbolMgr::Traverse(Visitor<Symbol>& visitor) const
{
	std::map<std::string, Symbol*>::const_iterator itr = m_syms.begin();
	for ( ; itr != m_syms.end(); ++itr)
	{
		bool next;
		visitor.Visit(itr->second, next);
		if (!next) break;
	}
}


}