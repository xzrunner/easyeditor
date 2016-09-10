#include "SymbolFactory.h"
#include "StringHelper.h"
#include "SymFileType.h"

#include "ImageSymbol.h"
#include "Scale9SymLoader.h"
#include "TextboxSymbol.h"
#include "ComplexSymLoader.h"
#include "AnimSymLoader.h"
#include "P3dSymLoader.h"
#include "P2dSymLoader.h"
#include "TrailSymLoader.h"

#include <sprite2/Scale9Symbol.h>
#include <sprite2/ComplexSymbol.h>
#include <sprite2/AnimSymbol.h>
#include <sprite2/Particle3dSymbol.h>
#include <sprite2/Particle2dSymbol.h>
#include <sprite2/TrailSymbol.h>

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
	
	SymFileType type = get_sym_file_type(filepath);
	switch (type)
	{
	case IMAGE:
		ret = new ImageSymbol(filepath);
		break;
	case SCALE9:
		{
			s2::Scale9Symbol* sym = new s2::Scale9Symbol();
			Scale9SymLoader loader(sym);
			loader.LoadJson(filepath);
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
	case COMPLEX:
		{
			s2::ComplexSymbol* sym = new s2::ComplexSymbol();
			ComplexSymLoader loader(sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case ANIMATION:
		{
			s2::AnimSymbol* sym = new s2::AnimSymbol();
			AnimSymLoader loader(sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case PARTICLE3D:
		{
			s2::Particle3dSymbol* sym = new s2::Particle3dSymbol;
			P3dSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case PARTICLE2D:
		{
			s2::Particle2dSymbol* sym = new s2::Particle2dSymbol;
			P2dSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case TRAIL:
		{
			s2::TrailSymbol* sym = new s2::TrailSymbol;
			TrailSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
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