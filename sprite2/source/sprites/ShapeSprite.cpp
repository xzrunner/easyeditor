#include "ShapeSprite.h"

#include <stddef.h>

namespace s2
{

ShapeSprite::ShapeSprite()
	: Sprite()
{
}

ShapeSprite* ShapeSprite::Clone() const
{
	return new ShapeSprite(*this);
}

}