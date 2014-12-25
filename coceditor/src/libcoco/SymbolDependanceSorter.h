#pragma once

#include <drag2d.h>

namespace libcoco
{

class SymbolDependanceSorter
{
public:

	void prepare(const std::vector<const d2d::ISymbol*>& symbols);
	void prepare(const std::vector<const d2d::ISprite*>& sprites);

	const std::vector<const d2d::ISymbol*>& GetAllSymbols() const {
		return m_result;
	}

private:
	void fetch(const std::vector<const d2d::ISymbol*>& symbols);
	void sort();

	bool isSymbolPrepared(const d2d::ISprite* sprite) const;

private:
	std::set<const d2d::ISymbol*, d2d::SymbolCmp> m_unique;

	std::vector<const d2d::ISymbol*> m_result;

}; // SymbolsSort

}