#include "IconSprite.h"

#include <stddef.h>

namespace s2
{

IconSprite::IconSprite()
{
}

IconSprite::IconSprite(const IconSprite& spr)
	: Sprite(spr)
{
}

IconSprite* IconSprite::Clone() const
{
	return new IconSprite(*this);
}

}