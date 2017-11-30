#include "SymbolMgr.h"
#include "SymbolFactory.h"
#include "Symbol.h"
#include "Exception.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "Visitor.h"
#include "SymbolType.h"

#include <gum/FilepathHelper.h>

namespace ee
{

CU_SINGLETON_DEFINITION(SymbolMgr);

SymbolMgr::SymbolMgr()
{
}

SymbolMgr::~SymbolMgr()
{
	Clear();
}

SymPtr SymbolMgr::FetchSymbol(const std::string& filepath, int type)
{
	std::string fixed_path = gum::FilepathHelper::Format(filepath.c_str()).c_str();

	std::string filename = FileHelper::GetFilename(filepath);
	auto itr = m_syms.find(fixed_path);
	if (filename != SYM_GROUP_TAG && itr != m_syms.end())
	{
		SymPtr ret = itr->second.lock();
		if (ret) {
			return ret;
		} else {
			m_syms.erase(itr);
		}
	}
		
	auto sym = SymbolFactory::Create(fixed_path, type);
	if (!sym) 
	{
		const char* path = filepath.c_str();
		throw Exception("Create symbol fail: %s", path);
//			return NULL;
	}

	bool succ = sym->LoadFromFile(fixed_path);
	if (succ)
	{
		m_syms.insert(std::make_pair(fixed_path, sym));
		return sym;
	}
	else
	{
		const char* path = filepath.c_str();
		printf("Load symbol %s fail!", path);
		throw Exception("Load symbol %s fail!", path);

//			return NULL;
	}
}

void SymbolMgr::Insert(SymPtr& sym)
{
	std::string fixed_path = gum::FilepathHelper::Format(sym->GetFilepath().c_str()).c_str();
	m_syms.insert(std::make_pair(fixed_path, sym));
}

void SymbolMgr::Remove(const SymPtr& sym)
{
	std::string lowerpath = sym->GetFilepath();
	StringHelper::ToLower(lowerpath);
	auto itr = m_syms.find(lowerpath);
	// todo: new DummySymbol()
//	assert(itr != m_syms.end());
	if (itr != m_syms.end()) {
		m_syms.erase(itr);
	}
}

void SymbolMgr::Clear()
{
	m_syms.clear();
}

void SymbolMgr::Traverse(RefVisitor<Symbol>& visitor) const
{
	for (auto& sym : m_syms)
	{
		bool next;
		auto s = sym.second.lock();
		if (s) {
			visitor.Visit(s, next);
			if (!next) break;
		}
	}

	auto itr = m_syms.begin();
	for ( ; itr != m_syms.end(); ++itr)
	{

	}
}


}