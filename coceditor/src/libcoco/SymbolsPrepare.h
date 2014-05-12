#pragma once

#include <drag2d.h>

namespace libcoco
{

class SymbolsPrepare
{
public:

	void prepare(const std::vector<const d2d::ISymbol*>& symbols);
	void prepare(const std::vector<const d2d::ISprite*>& sprites);

	const std::vector<const d2d::ISymbol*>& getResult() const {
		return m_result;
	}

private:
	void fetch(const std::vector<const d2d::ISymbol*>& symbols);
	void sort();

	bool isSymbolPrepared(const d2d::ISprite* sprite) const;

private:
	std::set<const d2d::ISymbol*> m_unique;

	std::vector<const d2d::ISymbol*> m_result;

}; // SymbolsSort

}