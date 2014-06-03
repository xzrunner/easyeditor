#ifndef _SG_SYMBOL_EXT_H_
#define _SG_SYMBOL_EXT_H_

#include "BuildingCfg.h"

namespace sg
{

struct SymbolExt
{
	int size;
	int remain;
	int wall_type; // -1 not wall, 0 single, 1 left, 2 right, 3 both

	int level;
	BuildingCfg::Building* building;

}; // SymbolInfo

}

#endif // _SG_SYMBOL_EXT_H_