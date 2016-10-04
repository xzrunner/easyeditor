#include "SymbolLoader.h"
#include "SymbolMgr.h"
#include "Symbol.h"

namespace ee
{

s2::Symbol* SymbolLoader::Create(const std::string& filepath, int type) const
{
	return SymbolMgr::Instance()->FetchSymbol(filepath, type);
}

}