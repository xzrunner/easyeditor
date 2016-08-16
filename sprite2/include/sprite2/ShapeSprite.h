#ifndef _SPRITE2_SHAPE_SPRITE_H_
#define _SPRITE2_SHAPE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ShapeSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ShapeSprite();
	ShapeSprite(const ShapeSprite& spr);
	ShapeSprite(Symbol* sym) : Sprite(sym) {}

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual ShapeSprite* Clone() const;

}; // ShapeSprite

}

#endif // _SPRITE2_SHAPE_SPRITE_H_