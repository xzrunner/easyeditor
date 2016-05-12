#ifndef _SPRITE2_SCALE9_SPRITE_H_
#define _SPRITE2_IMAGE_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class Scale9Symbol;

class Scale9Sprite : public Sprite
{
public:
	Scale9Sprite();
	
	virtual bool Update(float dt) { return false; }
	
private:
	Scale9Symbol* m_sym;

}; // Scale9Sprite

}

#endif // _SPRITE2_SCALE9_SPRITE_H_