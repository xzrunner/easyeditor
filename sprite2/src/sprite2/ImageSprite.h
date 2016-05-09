#ifndef _SPRITE2_IMAGE_SPRITE_H_
#define _SPRITE2_IMAGE_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class ImageSymbol;

class ImageSprite : public Sprite
{
public:
	ImageSprite();
	
	virtual bool Update(float dt) const { return false; }
	
private:
	ImageSymbol* m_sym;

}; // ImageSprite

}

#endif // _SPRITE2_IMAGE_SPRITE_H_