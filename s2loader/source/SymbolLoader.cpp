#include "s2loader/SymbolLoader.h"

#include <gum/SymbolPool.h>

namespace s2loader
{

s2::SymPtr SymbolLoader::Create(const CU_STR& filepath, int type) const
{
	return gum::SymbolPool::Instance()->Fetch(filepath, type);
}

s2::SymPtr SymbolLoader::Create(int type) const
{
	return nullptr;
}

}