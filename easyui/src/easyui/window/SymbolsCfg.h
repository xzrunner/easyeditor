#ifndef _EASYUI_WINDOW_SYMBOL_CFG_H_
#define _EASYUI_WINDOW_SYMBOL_CFG_H_

#include <json/json.h>

namespace ee { class LibraryPanel; }

namespace eui
{
namespace window
{

class LibraryPage;
class StagePanel;

class SymbolsCfg
{
public:
	SymbolsCfg(StagePanel* stage, ee::LibraryPanel* library);
	~SymbolsCfg();

	void LoadConfig();

private:
	struct Symbol
	{
		std::string filepath;
	}; // Symbol

private:
	void InitLibrarySymbols(const Json::Value& value);
	void InitLibrarySymbols(const Json::Value& value, const std::string& name, const std::string& filter = "");
	void InitLibrarySymbols(const Json::Value& value, std::vector<Symbol*>& symbols);

	void ResetLibraryList(LibraryPage* library, const std::vector<Symbol*>& symbols);

private:
	StagePanel* m_stage;

	ee::LibraryPanel* m_library;

	std::vector<Symbol*> m_symbols;

}; // SymbolsCfg

}
}

#endif // _EASYUI_WINDOW_SYMBOL_CFG_H_