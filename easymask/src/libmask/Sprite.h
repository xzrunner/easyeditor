#ifndef _EASYMASK_SPRITE_H_
#define _EASYMASK_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/DummySprite.h>

namespace emask
{

class Symbol;

class Sprite : public s2::DummySprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual bool Update(const s2::RenderParams& params, float dt) { return true; }

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual Sprite* EEClone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* symbol);

}; // Sprite

}

#endif // _EASYMASK_SPRITE_H_