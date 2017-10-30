#ifndef _EASYEDITOR_SYMBOL_PATH_H_
#define _EASYEDITOR_SYMBOL_PATH_H_

#include "Symbol.h"

#include <string>

namespace ee
{

class SymbolPath
{
public:
	static std::string GetRelativePath(const Symbol& sym, const std::string& dir);

}; // SymbolPath

}

#endif // _EASYEDITOR_SYMBOL_PATH_H_