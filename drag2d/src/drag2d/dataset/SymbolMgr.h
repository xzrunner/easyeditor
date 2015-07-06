#ifndef _DRAG2D_SYMBOL_MGR_H_
#define _DRAG2D_SYMBOL_MGR_H_

#include <map>

namespace d2d
{

class ISymbol;
class IVisitor;

class SymbolMgr
{
public:
	static SymbolMgr* Instance();

	ISymbol* FetchSymbol(const std::string& filepath);

	void Clear();

	void Traverse(IVisitor& visitor) const;

	int Size() const {
		return m_symbols.size();
	}

protected:
	void Remove(const ISymbol* symbol);

private:
	SymbolMgr();
	~SymbolMgr();

private:
	static SymbolMgr* m_instance;

	std::map<std::string, ISymbol*> m_symbols;

	friend class ISymbol;

}; // SymbolMgr

}

#endif // _DRAG2D_SYMBOL_MGR_H_