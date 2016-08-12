#include "TextureSprite.h"

#include <stddef.h>

namespace s2
{

TextureSprite::TextureSprite()
{
}

TextureSprite::TextureSprite(const TextureSprite& spr)
	: Sprite(spr)
{
}

TextureSprite* TextureSprite::Clone() const
{
	return new TextureSprite(*this);
}

}