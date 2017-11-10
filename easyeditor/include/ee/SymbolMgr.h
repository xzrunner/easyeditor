#ifndef _EASYEDITOR_SYMBOL_MGR_H_
#define _EASYEDITOR_SYMBOL_MGR_H_

#include "Visitor.h"
#include "Symbol.h"

#include <string>
#include <cu/cu_macro.h>
#include <sprite2/SymType.h>

#include <map>

namespace ee
{

class SymbolMgr
{
public:
	SymPtr FetchSymbol(const std::string& filepath, int type = s2::SYM_UNKNOWN);

	void Insert(SymPtr& sym);

	void Clear();

	void Traverse(RefVisitor<Symbol>& visitor) const;

	int Size() const {
		return m_syms.size();
	}

protected:
	void Remove(const SymPtr& sym);

private:
	std::map<std::string, std::weak_ptr<Symbol>> m_syms;

	CU_SINGLETON_DECLARATION(SymbolMgr);

	friend class Symbol;

}; // SymbolMgr

}

#endif // _EASYEDITOR_SYMBOL_MGR_H_