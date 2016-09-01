#ifndef _EASYP3DINV_SPRITE_H_
#define _EASYP3DINV_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/DummySprite.h>

namespace ep3dinv
{

class Symbol;

class Sprite : public s2::DummySprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);
	virtual ~Sprite();

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _EASYP3DINV_SPRITE_H_