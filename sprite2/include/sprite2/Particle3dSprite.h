#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class Particle3dSprite : public Sprite
{
public:
	Particle3dSprite(void* ud);
	Particle3dSprite(const Particle3dSprite& spr, void* ud);

	virtual bool Update(float dt) { return false; }
	
private:
	Particle3dSprite() {}
	const Particle3dSprite& operator = (const Particle3dSprite& spr) { return *this; }

}; // Particle3dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_