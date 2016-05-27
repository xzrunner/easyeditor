#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class Particle2dSprite : public Sprite
{
public:
	Particle2dSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	Particle2dSprite() {}
	const Particle2dSprite& operator = (const Particle2dSprite& spr) { return *this; }

}; // Particle2dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_