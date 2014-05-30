#ifndef _SG_SYMBOL_INFO_H_
#define _SG_SYMBOL_INFO_H_

#include "ResourceMgr.h"

namespace sg
{

struct SymbolInfo
{
	int size;

	// todo
	int level;
	ResourceMgr::Building* building;

}; // SymbolInfo

}

#endif // _SG_SYMBOL_INFO_H_