#include "MeshSprite.h"

#include "MeshTriangle.h"

#include <stddef.h>

namespace s2
{

MeshSprite::MeshSprite() 
{
}

MeshSprite::MeshSprite(Symbol* sym) 
	: Sprite(sym) 
{
}

MeshSprite* MeshSprite::Clone() const
{
	return new MeshSprite(*this);
}

}