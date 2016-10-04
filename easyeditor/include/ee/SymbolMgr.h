#ifndef _EASYEDITOR_SYMBOL_MGR_H_
#define _EASYEDITOR_SYMBOL_MGR_H_

#include "Visitor.h"

#include <sprite2/SymType.h>

#include <map>

namespace ee
{

class Symbol;

class SymbolMgr
{
public:
	static SymbolMgr* Instance();

	Symbol* FetchSymbol(const std::string& filepath, int type = s2::SYM_UNKNOWN);

	void Clear();

	void Traverse(Visitor<Symbol>& visitor) const;

	int Size() const {
		return m_syms.size();
	}

protected:
	void Remove(const Symbol* sym);

private:
	SymbolMgr();
	~SymbolMgr();

private:
	static SymbolMgr* m_instance;

	std::map<std::string, Symbol*> m_syms;

	friend class Symbol;

}; // SymbolMgr

}

#endif // _EASYEDITOR_SYMBOL_MGR_H_