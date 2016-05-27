#ifndef _SPRITE2_IMAGE_SPRITE_H_
#define _SPRITE2_IMAGE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ImageSprite : public Sprite
{
public:
	ImageSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	ImageSprite() {}
	const ImageSprite& operator = (const ImageSprite& spr) { return *this; }

}; // ImageSprite

}

#endif // _SPRITE2_IMAGE_SPRITE_H_