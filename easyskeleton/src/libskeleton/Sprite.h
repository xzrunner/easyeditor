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
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _LIBSKELETON_SPRITE_H_
