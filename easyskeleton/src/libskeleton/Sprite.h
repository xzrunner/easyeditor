#ifndef _LIBSKELETON_SPRITE_H_
#define _LIBSKELETON_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/SkeletonSprite.h>

namespace libskeleton
{

class Symbol;

class Sprite : public s2::SkeletonSprite, public ee::Sprite
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
	virtual void Store(Json::Value& val, const CU_STR& dir = "") const;

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _LIBSKELETON_SPRITE_H_
