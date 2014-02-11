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

void AnimSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		Rect rect = m_symbol->m_rect;

		rect.scale(m_scale.x, m_scale.y);
		rect.translate(m_pos);
		m_bounding->initFromRect(rect);
		m_bounding->setTransform(m_pos, m_offset, m_angle);
	}
}
} // d2d