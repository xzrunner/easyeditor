#include "SymbolFactory.h"
#include "StringHelper.h"
#include "SymbolFileType.h"

#include "ImageSymbol.h"
#include "Scale9Symbol.h"
#include "TextboxSymbol.h"

namespace glue
{

SINGLETON_DEFINITION(SymbolFactory);

SymbolFactory::SymbolFactory()
{
}

SymbolFactory::~SymbolFactory()
{
	std::map<std::string, s2::Symbol*>::iterator itr = m_path_cache.begin();
	for ( ; itr != m_path_cache.end(); ++itr) {
		itr->second->RemoveReference();
	}

	std::map<uint32_t, s2::Symbol*>::iterator itr2 = m_id_cache.begin();
	for ( ; itr2 != m_id_cache.end(); ++itr2) {
		itr2->second->RemoveReference();
	}
}

s2::Symbol* SymbolFactory::Create(const std::string& filepath) const
{
	std::string fixed_path = filepath;
	StringHelper::ToLower(fixed_path);

	std::map<std::string, s2::Symbol*>::const_iterator itr 
		= m_path_cache.find(fixed_path);
	if (itr != m_path_cache.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	s2::Symbol* ret = NULL;
	
	SymbolFileType type = get_sym_file_type(filepath);
	switch (type)
	{
	case IMAGE:
		ret = new ImageSymbol(filepath);
		break;
	case SCALE9:
		{
			Scale9Symbol* sym = new Scale9Symbol();
			sym->LoadJson(filepath);
			ret = sym;
		}
		break;
	case TEXTBOX:
		{
			TextboxSymbol* sym = new TextboxSymbol();
			sym->LoadJson(filepath);
			ret = sym;
		}
		break;
	}

	if (ret) {
		m_path_cache.insert(std::make_pair(fixed_path, ret));
		ret->AddReference();
	}

	return ret;
}

s2::Symbol* SymbolFactory::Create(uint32_t id) const
{
	std::map<uint32_t, s2::Symbol*>::const_iterator itr = m_id_cache.find(id);
	if (itr != m_id_cache.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	s2::Symbol* ret = NULL;

	return ret;
}

}