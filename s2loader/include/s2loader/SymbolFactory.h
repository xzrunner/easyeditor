#ifndef _S2LOADER_SYMBOL_FACTORY_H_
#define _S2LOADER_SYMBOL_FACTORY_H_

#include <cu/cu_macro.h>
#include <sprite2/SymType.h>
#include <sprite2/typedef.h>
#include <cu/cu_stl.h>

#include <stdint.h>

namespace s2 { class Symbol; }

namespace s2loader
{

class SymbolFactory
{
public:
	/**
	 *  @brief
	 *    raw, json res
	 */		
	s2::SymPtr Create(const CU_STR& filepath, int type = s2::SYM_UNKNOWN) const;

	/**
	 *  @brief
	 *    pack, binary res
	 */	
	s2::SymPtr Create(uint32_t id) const;
	s2::SymPtr Create(const CU_STR& pkg_name, const CU_STR& node_name) const;

private:
	mutable s2::SymPtr m_anchor_sym = nullptr;

	CU_SINGLETON_DECLARATION(SymbolFactory);

}; // SymbolFactory

}

#endif // _S2LOADER_SYMBOL_FACTORY_H_
