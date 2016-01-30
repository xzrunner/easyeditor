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

	void prepare(const std::vector<const d2d::Symbol*>& symbols);
	void prepare(const std::vector<const d2d::Sprite*>& sprites);

// 	const std::vector<const d2d::Symbol*>& GetSymbolOrdered() const {
// 		return m_symbol_set.GetOrdered();
// 	}
	const SymbolSet& GetSymbolSet() const { return m_symbol_set; }

private:
	void fetch(const std::vector<const d2d::Symbol*>& symbols);
	void sort();

	bool IsSymbolPrepared(const d2d::Sprite* sprite) const;
	bool IsSymbolPrepared(const d2d::Symbol* symbol) const;

	void PrepareScale9(std::queue<const d2d::Symbol*>& buffer, const escale9::Symbol* scale9);

private:
//	std::set<const d2d::Symbol*, d2d::SymbolCmp> m_unique;
	std::set<const d2d::Symbol*> m_unique;

	SymbolSet m_symbol_set;

}; // SymbolsSort

}

#endif // _LIBCOCO_SYMBOL_DEPENDANCE_SORTER_H_