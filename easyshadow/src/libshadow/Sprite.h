#ifndef _EASYSHADOW_SPRITE_H_
#define _EASYSHADOW_SPRITE_H_

#include <ee/Sprite.h>

namespace eshadow
{

class Symbol;

class Sprite : public ee::Sprite
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

#endif // _EASYSHADOW_SPRITE_H_