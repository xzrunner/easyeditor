#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class Particle2dSymbol;

class Particle2dSprite : public Sprite
{
public:
	Particle2dSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	Particle2dSymbol* m_sym;

}; // Particle2dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_