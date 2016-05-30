#ifndef _SPRITE2_SHAPE_SPRITE_H_
#define _SPRITE2_SHAPE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ShapeSprite : public Sprite
{
public:
	ShapeSprite(void* ud);
	ShapeSprite(const ShapeSprite& spr, void* ud);

	virtual bool Update(float dt) { return false; }
	
private:
	ShapeSprite() {}
	const ShapeSprite& operator = (const ShapeSprite& spr) { return *this; }

}; // ShapeSprite

}

#endif // _SPRITE2_SHAPE_SPRITE_H_