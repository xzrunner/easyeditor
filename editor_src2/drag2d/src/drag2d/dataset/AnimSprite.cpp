#include "AnimSprite.h"
#include "SpriteFactory.h"

#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

namespace d2d
{

AnimSprite::AnimSprite()
	: m_symbol(NULL)
{
}

AnimSprite::AnimSprite(const AnimSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

AnimSprite::AnimSprite(AnimSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

AnimSprite::~AnimSprite()
{
}

AnimSprite* AnimSprite::clone() const
{
	AnimSprite* sprite = new AnimSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite; 
}

const AnimSymbol& AnimSprite::getSymbol() const
{
	return *m_symbol;
}

void AnimSprite::setSymbol(ISymbol* symbol)
{
	AnimSymbol* anim = dynamic_cast<AnimSymbol*>(symbol);
	if (anim) m_symbol = anim;
}

void AnimSprite::loadBodyFromFile()
{
}

} // d2d