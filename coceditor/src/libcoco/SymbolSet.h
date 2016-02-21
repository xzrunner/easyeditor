#ifndef _ECOCO_SYMBOL_SET_H_
#define _ECOCO_SYMBOL_SET_H_

#include <string>
#include <vector>
#include <map>

namespace ee { class Symbol; }

namespace ecoco
{

class SymbolSet
{
public:

	void Insert(const ee::Symbol* symbol);

	const ee::Symbol* Query(const std::string& filepath) const;
	bool Query(const ee::Symbol* symbol) const;

	const std::vector<const ee::Symbol*>& GetOrdered() const { return m_symbol_ordered; }

private:
	std::multimap<std::string, const ee::Symbol*> m_symbol_map;

	std::vector<const ee::Symbol*> m_symbol_ordered;

}; // SymbolSet

}

#endif // _ECOCO_SYMBOL_SET_H_