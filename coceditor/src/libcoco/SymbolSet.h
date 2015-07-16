#ifndef _LIBCOCO_SYMBOL_SET_H_
#define _LIBCOCO_SYMBOL_SET_H_

#include <drag2d.h>

namespace libcoco
{

class SymbolSet
{
public:

	void Insert(const d2d::ISymbol* symbol);

	const d2d::ISymbol* Query(const std::string& filepath) const;
	bool Query(const d2d::ISymbol* symbol) const;

	const std::vector<const d2d::ISymbol*>& GetOrdered() const { return m_symbol_ordered; }

private:
	static std::string FormatFilepath(const std::string& filepath);

private:
	std::map<std::string, const d2d::ISymbol*> m_symbol_map;

	std::vector<const d2d::ISymbol*> m_symbol_ordered;

}; // SymbolSet

}

#endif // _LIBCOCO_SYMBOL_SET_H_