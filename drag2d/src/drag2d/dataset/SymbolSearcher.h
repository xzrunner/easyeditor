#ifndef _D2D_SYMBOL_SEARCHER_H_
#define _D2D_SYMBOL_SEARCHER_H_

#include <wx/string.h>
#include <JSON/json.h>

namespace d2d
{

class ISymbol;

class SymbolSearcher
{
public:
	static wxString GetSymbolPath(const wxString& dir, const Json::Value& sprite_val);

	static void SetSymbolFilepaths(const wxString& dir, ISymbol* symbol, 
		const Json::Value& sprite_val);

}; // SymbolSearcher

}

#endif // _D2D_SYMBOL_SEARCHER_H_