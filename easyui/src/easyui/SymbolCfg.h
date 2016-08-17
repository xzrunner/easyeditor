#ifndef _EASYUI_SYMBOL_CONFIG_H_
#define _EASYUI_SYMBOL_CONFIG_H_

namespace ee { class Symbol; }

#include <string>
#include <set>
#include <vector>

namespace eui
{

class SymbolCfg
{
public:
	void QueryType(const ee::Symbol* sym, std::string& type) const;

	static SymbolCfg* Instance();

private:
	SymbolCfg() {}

	void LoadFromFile(const char* filename);

private:
	struct Widget
	{
		std::string type;
		std::set<std::string> syms;
	};

private:
	std::vector<Widget> m_widgets;

private:
	static SymbolCfg* m_instance;

}; // SymbolCfg

}

#endif // _EASYUI_SYMBOL_CONFIG_H_