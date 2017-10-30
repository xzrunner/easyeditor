#ifndef _EASYTERRAIN2D_SPRITE_H_
#define _EASYTERRAIN2D_SPRITE_H_

#include <ee/Sprite.h>

namespace eterrain2d
{

class Symbol;

class Sprite : public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const CU_STR& dir = "");

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	SPRITE_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYTERRAIN2D_SPRITE_H_