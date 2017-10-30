#ifndef _ECOCO_SYMBOL_SET_H_
#define _ECOCO_SYMBOL_SET_H_

#include <ee/Symbol.h>

#include <string>
#include <vector>
#include <map>

namespace ee { class Symbol; }

namespace ecoco
{

class SymbolSet
{
public:

	void Insert(const ee::SymConstPtr& sym);

	ee::SymConstPtr Query(const std::string& filepath) const;
	bool Query(const ee::SymConstPtr& sym) const;

	auto& GetOrdered() const { return m_symbol_ordered; }

private:
	std::multimap<std::string, ee::SymConstPtr> m_symbol_map;

	std::vector<ee::SymConstPtr> m_symbol_ordered;

}; // SymbolSet

}

#endif // _ECOCO_SYMBOL_SET_H_