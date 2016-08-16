#ifndef _SPRITE2_SCALE9_SPRITE_H_
#define _SPRITE2_SCALE9_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class RenderParams;

class Scale9Sprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Scale9Sprite();
	Scale9Sprite(const Scale9Sprite& spr);
	Scale9Sprite(Symbol* sym) : Sprite(sym) {}

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Scale9Sprite* Clone() const;

	void Draw(const RenderParams& params) const;

private:
	float m_width, m_height;

	// 0 1 2
	// 3 4 5
	// 6 7 9
	Sprite* m_grids[9];

}; // Scale9Sprite

}

#endif // _SPRITE2_SCALE9_SPRITE_H_