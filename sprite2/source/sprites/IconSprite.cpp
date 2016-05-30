#include "IconSprite.h"

#include <stddef.h>

namespace s2
{

IconSprite::IconSprite(void* ud)
	: Sprite(ud)
{
}

IconSprite::IconSprite(const IconSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}