#ifndef _EASYTRAIL_SPRITE_H_
#define _EASYTRAIL_SPRITE_H_

#include "TrailNode.h"

#include <ee/Sprite.h>

#include <sprite2/DummySprite.h>

namespace etrail
{

class Symbol;

class Sprite : public s2::DummySprite, public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
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

	void Draw(const s2::RenderParams& params) const;

	static ee::Sprite* Create(ee::Symbol* symbol);

private:
	t2d_emitter* m_et;

	mutable RenderParams m_rp;

}; // Sprite

}

#endif // _EASYTRAIL_SPRITE_H_