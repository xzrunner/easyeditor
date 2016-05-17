#ifndef _SPRITE2_COMPLEX_SPRITE_H_
#define _SPRITE2_COMPLEX_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class ComplexSymbol;

class ComplexSprite : public Sprite
{
public:
	ComplexSprite(void* ud);
	
	virtual bool Update(float dt);
	
private:
	ComplexSymbol* m_sym;

}; // ComplexSprite

}

#endif // _SPRITE2_COMPLEX_SPRITE_H_