#ifndef _EASYTERRAIN2D_SPRITE_H_
#define _EASYTERRAIN2D_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/DummySprite.h>

namespace eterrain2d
{

class Symbol;

class Sprite : public s2::DummySprite, public ee::Sprite
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

	virtual void Load(const Json::Value& val, const std::string& dir = "");

	static ee::Sprite* Create(ee::Symbol* symbol);

}; // Sprite

}

#endif // _EASYTERRAIN2D_SPRITE_H_