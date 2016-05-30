#include "TextboxSprite.h"

#include <stddef.h>

namespace s2
{

TextboxSprite::TextboxSprite(void* ud)
	: Sprite(ud)
{
}

TextboxSprite::TextboxSprite(const TextboxSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}