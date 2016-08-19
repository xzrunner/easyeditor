#include "AnimSprite.h"

namespace s2
{

AnimSprite::AnimSprite() 
{
}

AnimSprite::AnimSprite(Symbol* sym) 
	: Sprite(sym)
{
}

AnimSprite* AnimSprite::Clone() const
{
	return new AnimSprite(*this);
}

}