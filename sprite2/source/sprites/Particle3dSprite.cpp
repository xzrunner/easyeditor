#include "Particle3dSprite.h"

#include <stddef.h>

namespace s2
{

Particle3dSprite::Particle3dSprite(void* ud)
	: Sprite(ud)
{
}

Particle3dSprite::Particle3dSprite(const Particle3dSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}