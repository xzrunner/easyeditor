#include "ShapeSprite.h"

#include <stddef.h>

namespace s2
{

ShapeSprite::ShapeSprite(void* ud)
	: Sprite(ud)
{
}

ShapeSprite::ShapeSprite(const ShapeSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}