#include "ImageSprite.h"

#include <stddef.h>

namespace s2
{

ImageSprite::ImageSprite()
{
}

ImageSprite::ImageSprite(Symbol* sym) 
	: Sprite(sym) 
{
}

ImageSprite* ImageSprite::Clone() const
{
	return new ImageSprite(*this);
}

}