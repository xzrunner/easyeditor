#include "Particle3dSprite.h"

#include <stddef.h>

namespace s2
{

Particle3dSprite::Particle3dSprite()
	: Sprite()
{
}

Particle3dSprite::Particle3dSprite(const Particle3dSprite& spr)
	: Sprite(spr)
{
}

Particle3dSprite* Particle3dSprite::Clone() const
{
	return new Particle3dSprite(*this);
}

}