#include "Particle3dSprite.h"

#include <stddef.h>

namespace s2
{

Particle3dSprite::Particle3dSprite() 
{
}

Particle3dSprite::Particle3dSprite(Symbol* sym) 
	: Sprite(sym) 
{
}

Particle3dSprite* Particle3dSprite::Clone() const
{
	return new Particle3dSprite(*this);
}

}