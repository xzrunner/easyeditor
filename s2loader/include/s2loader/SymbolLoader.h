#ifndef _S2LOADER_SYMBOL_LOADER_H_
#define _S2LOADER_SYMBOL_LOADER_H_

#include <sprite2/SymType.h>
#include <sprite2/typedef.h>
#include <cu/cu_stl.h>

namespace s2loader
{

class SymbolLoader
{
public:
	virtual s2::SymPtr Create(const CU_STR& filepath, int type = s2::SYM_UNKNOWN) const;
	virtual s2::SymPtr Create(int type) const;

}; // SymbolLoader

}

#endif // _S2LOADER_SYMBOL_LOADER_H_
