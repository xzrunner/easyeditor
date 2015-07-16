#ifndef _LIBCOCO_SYMBOL_DEPENDANCE_SORTER_H_
#define _LIBCOCO_SYMBOL_DEPENDANCE_SORTER_H_

#include "SymbolSet.h"

#include <drag2d.h>
#include <queue>

namespace escale9 { class Symbol; }

namespace libcoco
{

class SymbolDependanceSorter
{
public:

	void prepare(const std::vector<const d2d::ISymbol*>& symbols);
	void prepare(const std::vector<const d2d::ISprite*>& sprites);

// 	const std::vector<const d2d::ISymbol*>& GetSymbolOrdered() const {
// 		return m_symbol_set.GetOrdered();
// 	}
	const SymbolSet& GetSymbolSet() const { return m_symbol_set; }

private:
	void fetch(const std::vector<const d2d::ISymbol*>& symbols);
	void sort();

	bool IsSymbolPrepared(const d2d::ISprite* sprite) const;
	bool IsSymbolPrepared(const d2d::ISymbol* symbol) const;

	void PrepareScale9(std::queue<const d2d::ISymbol*>& buffer, const escale9::Symbol* scale9);

private:
//	std::set<const d2d::ISymbol*, d2d::SymbolCmp> m_unique;
	std::set<const d2d::ISymbol*> m_unique;

	SymbolSet m_symbol_set;

}; // SymbolsSort

}

#endif // _LIBCOCO_SYMBOL_DEPENDANCE_SORTER_H_