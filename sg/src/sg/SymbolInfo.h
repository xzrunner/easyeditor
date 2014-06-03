#ifndef _SG_SYMBOL_INFO_H_
#define _SG_SYMBOL_INFO_H_

#include "ResourceMgr.h"

namespace sg
{

struct SymbolInfo
{
	int size;
	int remain;
//	bool iswall;
	int wall_type; // -1 not wall, 0 single, 1 left, 2 right, 3 both

	// todo
	int level;
	ResourceMgr::Building* building;

}; // SymbolInfo

}

#endif // _SG_SYMBOL_INFO_H_