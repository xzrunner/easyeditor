#ifndef _SPRITE2_ANIM_SPRITE_H_
#define _SPRITE2_ANIM_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class AnimSymbol;

class AnimSprite : public Sprite
{
public:
	AnimSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	AnimSymbol* m_sym;

}; // AnimSprite

}

#endif // _SPRITE2_ANIM_SPRITE_H_