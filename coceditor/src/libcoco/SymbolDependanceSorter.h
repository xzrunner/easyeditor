#ifndef _ECOCO_SYMBOL_DEPENDANCE_SORTER_H_
#define _ECOCO_SYMBOL_DEPENDANCE_SORTER_H_

#include "SymbolSet.h"

#include <queue>
#include <set>

namespace escale9 { class Symbol; }
namespace ee { class Sprite; }

namespace ecoco
{

class SymbolDependanceSorter
{
public:

	void prepare(const std::vector<const ee::Symbol*>& syms);
	void prepare(const std::vector<const ee::Sprite*>& sprs);

// 	const std::vector<const ee::Symbol*>& GetSymbolOrdered() const {
// 		return m_symbol_set.GetOrdered();
// 	}
	const SymbolSet& GetSymbolSet() const { return m_symbol_set; }

private:
	void fetch(const std::vector<const ee::Symbol*>& syms);
	void sort();

	bool IsSymbolPrepared(const ee::Sprite* spr) const;
	bool IsSymbolPrepared(const ee::Symbol* sym) const;

	void PrepareScale9(std::queue<const ee::Symbol*>& buffer, const escale9::Symbol* scale9);

private:
//	std::set<const ee::Symbol*, ee::SymbolCmp> m_unique;
	std::set<const ee::Symbol*> m_unique;

	SymbolSet m_symbol_set;

}; // SymbolsSort

}

#endif // _ECOCO_SYMBOL_DEPENDANCE_SORTER_H_