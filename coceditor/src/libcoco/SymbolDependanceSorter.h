#ifndef _ECOCO_SYMBOL_DEPENDANCE_SORTER_H_
#define _ECOCO_SYMBOL_DEPENDANCE_SORTER_H_

#include "SymbolSet.h"

#include <ee/Symbol.h>
#include <ee/Sprite.h>

#include <queue>
#include <set>

namespace escale9 { class Symbol; }

namespace ecoco
{

class SymbolDependanceSorter
{
public:

	void prepare(const std::vector<ee::SymPtr>& syms);
	void prepare(const std::vector<ee::SprPtr>& sprs);

// 	const std::vector<ee::SymPtr>& GetSymbolOrdered() const {
// 		return m_symbol_set.GetOrdered();
// 	}
	const SymbolSet& GetSymbolSet() const { return m_symbol_set; }

private:
	void fetch(const std::vector<ee::SymPtr>& syms);
	void sort();

	bool IsSymbolPrepared(const ee::SprConstPtr& spr) const;
	bool IsSymbolPrepared(const ee::SymConstPtr& sym) const;

	void PrepareScale9(std::queue<ee::SymPtr>& buffer, const std::shared_ptr<escale9::Symbol>& scale9);

private:
//	std::set<const ee::SymPtr&, ee::SymbolCmp> m_unique;
	std::set<ee::SymPtr> m_unique;

	SymbolSet m_symbol_set;

}; // SymbolsSort

}

#endif // _ECOCO_SYMBOL_DEPENDANCE_SORTER_H_