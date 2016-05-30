#include "ImageSprite.h"

#include <stddef.h>

namespace s2
{

ImageSprite::ImageSprite(void* ud)
	: Sprite(ud)
{
}

ImageSprite::ImageSprite(const ImageSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}