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
	virtual s2::Symbol* Create(const std::string& filepath, int type = s2::SYM_UNKNOWN) const;
	virtual s2::Symbol* Create(int type) const;

}; // SymbolLoader

}

#endif // _EASYEDITOR_SYMBOL_LOADER_H_