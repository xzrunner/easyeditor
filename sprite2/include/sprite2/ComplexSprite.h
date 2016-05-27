#ifndef _SPRITE2_COMPLEX_SPRITE_H_
#define _SPRITE2_COMPLEX_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ComplexSprite : public Sprite
{
public:
	ComplexSprite(void* ud);
	
	virtual bool Update(float dt);
	
private:
	ComplexSprite() {}
	const ComplexSprite& operator = (const ComplexSprite& spr) { return *this; }

}; // ComplexSprite

}

#endif // _SPRITE2_COMPLEX_SPRITE_H_