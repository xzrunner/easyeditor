#ifndef _EASYEDITOR_SYMBOL_SEARCHER_H_
#define _EASYEDITOR_SYMBOL_SEARCHER_H_

#include <json/json.h>

namespace ee
{

class Symbol;

class SymbolSearcher
{
public:
	static std::string GetSymbolPath(const std::string& dir, const Json::Value& sprite_val);

	static void SetSymbolFilepaths(const std::string& dir, Symbol* sym, 
		const Json::Value& sprite_val);

}; // SymbolSearcher

}

#endif // _EASYEDITOR_SYMBOL_SEARCHER_H_