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
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }
	virtual bool Update(const s2::RenderParams& params, float dt);

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _EASYMASK_SPRITE_H_