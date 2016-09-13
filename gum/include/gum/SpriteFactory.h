#ifndef _GUM_SPRITE_FACTORY_H_
#define _GUM_SPRITE_FACTORY_H_

#include <CU_Singleton.h>

#include <json/json.h>

#include <string>

namespace s2 { class Sprite; class Symbol; }

namespace gum
{

class SpriteFactory
{
public:
	/**
	 *  @brief
	 *    json
	 */	
	s2::Sprite* Create(const std::string& filepath) const;
	s2::Sprite* Create(const Json::Value& val, const std::string& dir) const;

	/**
	 *  @brief
	 *    binary
	 */	
	s2::Sprite* Create(const void* node, int type);

	SINGLETON_DECLARATION(SpriteFactory);

}; // SpriteFactory

}

#endif // _GUM_SPRITE_FACTORY_H_
