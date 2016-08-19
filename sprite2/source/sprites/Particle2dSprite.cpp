#include "Particle2dSprite.h"

#include <stddef.h>

namespace s2
{

Particle2dSprite::Particle2dSprite() 
{
}

Particle2dSprite::Particle2dSprite(Symbol* sym) 
	: Sprite(sym) 
{
}

Particle2dSprite* Particle2dSprite::Clone() const
{
	return new Particle2dSprite(*this);
}

}