#include "ShapeSprite.h"

#include <stddef.h>

namespace s2
{

ShapeSprite::ShapeSprite() 
{
}

ShapeSprite::ShapeSprite(Symbol* sym) 
	: Sprite(sym) 
{
}

ShapeSprite* ShapeSprite::Clone() const
{
	return new ShapeSprite(*this);
}

}