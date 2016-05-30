#include "AnimSprite.h"

namespace s2
{

AnimSprite::AnimSprite(void* ud)
	: Sprite(ud)
{
}

AnimSprite::AnimSprite(const AnimSprite& spr, void* ud)
	: Sprite(spr, ud)
{
}

}