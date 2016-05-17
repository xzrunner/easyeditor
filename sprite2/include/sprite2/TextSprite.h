#ifndef _SPRITE2_TEXT_SPRITE_H_
#define _SPRITE2_TEXT_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class TextSymbol;

class TextSprite : public Sprite
{
public:
	TextSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	TextSymbol* m_sym;

}; // TextSprite

}

#endif // _SPRITE2_TEXT_SPRITE_H_