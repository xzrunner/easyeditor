#include "ShapeSprite.h"

#include <stddef.h>

namespace s2
{

ShapeSprite::ShapeSprite()
	: Sprite()
{
}

ShapeSprite::ShapeSprite(const ShapeSprite& spr)
	: Sprite(spr)
{
}

ShapeSprite* ShapeSprite::Clone() const
{
	return new ShapeSprite(*this);
}

}