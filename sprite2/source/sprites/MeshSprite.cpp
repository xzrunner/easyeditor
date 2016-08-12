#include "MeshSprite.h"

#include <stddef.h>

namespace s2
{

MeshSprite::MeshSprite()
{
}

MeshSprite::MeshSprite(const MeshSprite& spr)
	: Sprite(spr)
{
}

MeshSprite* MeshSprite::Clone() const
{
	return new MeshSprite(*this);
}

}