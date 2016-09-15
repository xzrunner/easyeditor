#ifndef _GUM_SYMBOL_FACTORY_H_
#define _GUM_SYMBOL_FACTORY_H_

#include "SymFileType.h"

#include <CU_Singleton.h>

#include <string>
#include <map>

#include <stdint.h>

namespace s2 { class Symbol; }

namespace gum
{

class SymbolFactory
{
public:
	/**
	 *  @brief
	 *    raw, json res
	 */		
	s2::Symbol* Create(const std::string& filepath, SymFileType* type = NULL) const;

	/**
	 *  @brief
	 *    pack, binary res
	 */	
	s2::Symbol* Create(uint32_t id, SymFileType* type = NULL) const;
	s2::Symbol* Create(const std::string& pkg_name, const std::string& node_name, SymFileType* type = NULL) const;

private:
	mutable std::map<std::string, s2::Symbol*> m_path_cache;
	mutable std::map<uint32_t, s2::Symbol*> m_id_cache;

	SINGLETON_DECLARATION(SymbolFactory);

}; // SymbolFactory

}

#endif // _GUM_SYMBOL_FACTORY_H_
