#ifndef _EASYTEXTURE_SPRITE_H_
#define _EASYTEXTURE_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/TextureSprite.h>

namespace etexture
{

class Symbol;

class Sprite : public s2::TextureSprite, public ee::Sprite
{
public:
	Sprite(Symbol* symbol);

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

	static ee::Sprite* Create(ee::Symbol* symbol);

}; // Sprite

}

#endif // _EASYTEXTURE_SPRITE_H_