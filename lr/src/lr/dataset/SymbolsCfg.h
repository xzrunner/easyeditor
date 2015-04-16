#ifndef _LR_SYMBOL_CFG_H_
#define _LR_SYMBOL_CFG_H_

#include <drag2d.h>

namespace lr
{

class LibraryPage;
class StagePanel;

class SymbolsCfg
{
public:
	SymbolsCfg(StagePanel* stage, d2d::LibraryPanel* library);

	void LoadConfig();

private:
	struct Symbol
	{
		std::string filepath;
	}; // Symbol

	struct AllSymbol
	{
		std::vector<Symbol*> terrain;
		std::vector<Symbol*> unit;
		std::vector<Symbol*> decoration;		
	}; // AllSymbol

private:
	void InitLibrarySymbols(const Json::Value& value);
	void InitLibrarySymbols(const Json::Value& value, std::vector<Symbol*>& symbols);

	void ResetLibraryList(LibraryPage* library, const std::vector<Symbol*>& symbols);

private:
	StagePanel* m_stage;

	d2d::LibraryPanel* m_library;

	AllSymbol m_symbols;

}; // SymbolsCfg

}

#endif // _LR_SYMBOL_CFG_H_