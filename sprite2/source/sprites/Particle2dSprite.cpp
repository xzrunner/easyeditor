#include "Particle2dSprite.h"

#include <stddef.h>

namespace s2
{

Particle2dSprite::Particle2dSprite(void* ud)
	: Sprite(ud)
{
}

Particle2dSprite::Particle2dSprite(const Particle2dSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}