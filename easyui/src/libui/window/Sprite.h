#ifndef _EASYUI_WINDOW_SPRITE_H_
#define _EASYUI_WINDOW_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/DummySprite.h>

namespace eui
{
namespace window
{

class Symbol;

class Sprite : s2::DummySprite, public ee::Sprite
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
	virtual bool Update(const s2::RenderParams& params, float dt) { return true; }

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}
}

#endif // _EASYUI_WINDOW_SPRITE_H_