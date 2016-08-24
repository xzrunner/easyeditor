#ifndef _EASYCOMPLEX_SPRITE_H_
#define _EASYCOMPLEX_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/ComplexSprite.h>

namespace ecomplex
{

class Symbol;

class Sprite : public s2::ComplexSprite, public ee::Sprite
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

#endif // _EASYCOMPLEX_SPRITE_H_