#include "AnimSprite.h"

namespace s2
{

AnimSprite::AnimSprite()
{
}

AnimSprite::AnimSprite(const AnimSprite& spr)
	: Sprite(spr)
{
}

AnimSprite* AnimSprite::Clone() const
{
	return new AnimSprite(*this);
}

}