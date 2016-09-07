#ifndef _GLUE_SPRITE_FACTORY_H_
#define _GLUE_SPRITE_FACTORY_H_

#include <CU_Singleton.h>

#include <json/json.h>

#include <string>

namespace s2 { class Sprite; class Symbol; }

namespace glue
{

class SpriteFactory
{
public:
	s2::Sprite* Create(const std::string& filepath) const;
	s2::Sprite* Create(const Json::Value& val, const std::string& dir) const;

private:

	SINGLETON_DECLARATION(SpriteFactory);

}; // SpriteFactory

}

#endif // _GLUE_SPRITE_FACTORY_H_