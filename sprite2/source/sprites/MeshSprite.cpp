#include "MeshSprite.h"

#include <stddef.h>

namespace s2
{

MeshSprite::MeshSprite(void* ud)
	: Sprite(ud)
{
}

MeshSprite::MeshSprite(const MeshSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}