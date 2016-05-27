#ifndef _SPRITE2_ICON_SPRITE_H_
#define _SPRITE2_ICON_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class IconSprite : public Sprite
{
public:
	IconSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	IconSprite() {}
	const IconSprite& operator = (const IconSprite& spr) { return *this; }

}; // IconSprite

}

#endif // _SPRITE2_ICON_SPRITE_H_