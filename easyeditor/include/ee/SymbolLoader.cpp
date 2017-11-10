#include "SymbolLoader.h"
#include "SymbolMgr.h"
#include "Symbol.h"
#include "SymbolFactory.h"

#include <gum/StringHelper.h>

namespace ee
{

s2::SymPtr SymbolLoader::Create(const CU_STR& filepath, int type) const
{
	return SymbolMgr::Instance()->FetchSymbol(filepath.c_str(), type);
}

s2::SymPtr SymbolLoader::Create(int type) const
{
	static int IDX = 0;
	auto sym = SymbolFactory::Create(type);
	sym->SetFilepath((CU_STR("_no_filepath_sym_") + gum::StringHelper::ToString(IDX++)).c_str());
	SymbolMgr::Instance()->Insert(sym);
	return sym;
}

}