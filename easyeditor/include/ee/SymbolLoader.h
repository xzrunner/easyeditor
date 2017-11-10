#ifndef _EASYEDITOR_SYMBOL_LOADER_H_
#define _EASYEDITOR_SYMBOL_LOADER_H_

#include <gum/SymbolLoader.h>

namespace ee
{

class SymbolLoader : public gum::SymbolLoader
{
public:
	/**
	 *  @interface
	 *    gum::SymbolLoader
	 */		
	virtual s2::SymPtr Create(const CU_STR& filepath, int type = s2::SYM_UNKNOWN) const override;
	virtual s2::SymPtr Create(int type) const override;

	static bool IsNoFilepathSym(const std::string& filepath);

}; // SymbolLoader

}

#endif // _EASYEDITOR_SYMBOL_LOADER_H_