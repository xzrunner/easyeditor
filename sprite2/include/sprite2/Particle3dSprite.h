#ifndef _SPRITE2_PARTICLE3D_SPRITE_H_
#define _SPRITE2_PARTICLE3D_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class Particle3dSymbol;

class Particle3dSprite : public Sprite
{
public:
	Particle3dSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }
	
private:
	Particle3dSymbol* m_sym;

}; // Particle3dSprite

}

#endif // _SPRITE2_PARTICLE3D_SPRITE_H_