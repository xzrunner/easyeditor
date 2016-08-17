#ifndef _EASYSHAPE_SPRITE_H_
#define _EASYSHAPE_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/ShapeSprite.h>

namespace eshape
{

class Symbol;

class Sprite : public s2::ShapeSprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual Sprite* EEClone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _EASYSHAPE_SPRITE_H_