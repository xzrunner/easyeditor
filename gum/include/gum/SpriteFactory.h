#ifndef _GUM_SPRITE_FACTORY_H_
#define _GUM_SPRITE_FACTORY_H_

#include "SymFileType.h"

#include <CU_Singleton.h>

#include <json/json.h>

#include <string>

#include <stdint.h>

namespace s2 { class Sprite; class Symbol; }

namespace gum
{

class SpriteFactory
{
public:
	s2::Sprite* Create(s2::Symbol* sym, SymFileType type) const;

	/**
	 *  @brief
	 *    raw, json res
	 */	
	s2::Sprite* Create(const std::string& filepath) const;
	s2::Sprite* Create(const Json::Value& val, const std::string& dir) const;

	/**
	 *  @brief
	 *    pack, binary res
	 */	
	s2::Sprite* Create(uint32_t id);

	SINGLETON_DECLARATION(SpriteFactory);

}; // SpriteFactory

}

#endif // _GUM_SPRITE_FACTORY_H_
