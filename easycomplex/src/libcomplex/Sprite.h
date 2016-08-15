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
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual bool Update(const s2::RenderParams& params, float dt);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual Sprite* EEClone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _EASYCOMPLEX_SPRITE_H_