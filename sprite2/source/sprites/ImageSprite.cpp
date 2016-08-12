#include "ImageSprite.h"

#include <stddef.h>

namespace s2
{

ImageSprite::ImageSprite()
{
}

ImageSprite::ImageSprite(const ImageSprite& spr)
	: Sprite(spr)
{
}

ImageSprite* ImageSprite::Clone() const
{
	return new ImageSprite(*this);
}

}