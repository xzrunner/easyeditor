#pragma once

#include <drag2d.h>

#include <queue>

namespace escale9 { class Sprite; }

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

	bool IsSymbolPrepared(const d2d::ISprite* sprite) const;
	bool IsSymbolPrepared(const d2d::ISymbol* symbol) const;

	void PrepareScale9(std::queue<d2d::ISprite*>& buffer, const escale9::Sprite* scale9);

private:
//	std::set<const d2d::ISymbol*, d2d::SymbolCmp> m_unique;
	std::set<const d2d::ISymbol*> m_unique;

	std::vector<const d2d::ISymbol*> m_result;

}; // SymbolsSort

}