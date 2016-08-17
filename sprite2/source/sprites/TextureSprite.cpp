#include "TextureSprite.h"

#include <stddef.h>

namespace s2
{

TextureSprite::TextureSprite()
{
}

TextureSprite* TextureSprite::Clone() const
{
	return new TextureSprite(*this);
}

}