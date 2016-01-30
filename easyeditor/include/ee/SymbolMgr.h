#ifndef _EASYEDITOR_SYMBOL_MGR_H_
#define _EASYEDITOR_SYMBOL_MGR_H_

#include <map>

namespace ee
{

class Symbol;
class Visitor;

class SymbolMgr
{
public:
	static SymbolMgr* Instance();

	Symbol* FetchSymbol(const std::string& filepath);

	void Clear();

	void Traverse(Visitor& visitor) const;

	int Size() const {
		return m_symbols.size();
	}

protected:
	void Remove(const Symbol* symbol);

private:
	SymbolMgr();
	~SymbolMgr();

private:
	static SymbolMgr* m_instance;

	std::map<std::string, Symbol*> m_symbols;

	friend class Symbol;

}; // SymbolMgr

}

#endif // _EASYEDITOR_SYMBOL_MGR_H_