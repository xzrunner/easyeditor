#ifndef _SPRITE2_TEXT_SPRITE_H_
#define _SPRITE2_TEXT_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class TextSprite : public Sprite
{
public:
	TextSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	TextSprite() {}
	const TextSprite& operator = (const TextSprite& spr) { return *this; }

}; // TextSprite

}

#endif // _SPRITE2_TEXT_SPRITE_H_