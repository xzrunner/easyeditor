#include "Patch9Sprite.h"
#include "SpriteFactory.h"

#include "dataset/AbstractBV.h"
#include "dataset/BVFactory.h"

namespace d2d
{

Patch9Sprite::Patch9Sprite()
	: m_symbol(NULL)
{
	width = height = 0;
}

Patch9Sprite::Patch9Sprite(const Patch9Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	width = m_symbol->getWidth();
	height = m_symbol->getHeight();
}

Patch9Sprite::Patch9Sprite(Patch9Symbol* symbol)
	: m_symbol(symbol)
{
	width = m_symbol->getWidth();
	height = m_symbol->getHeight();
	buildBounding();
}

Patch9Sprite::~Patch9Sprite()
{
}

Patch9Sprite* Patch9Sprite::clone() const
{
	Patch9Sprite* sprite = new Patch9Sprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Patch9Symbol& Patch9Sprite::getSymbol() const
{
	return *m_symbol;
}

void Patch9Sprite::setSymbol(ISymbol* symbol)
{
	Patch9Symbol* anim = dynamic_cast<Patch9Symbol*>(symbol);
	if (anim) m_symbol = anim;
}

void Patch9Sprite::loadBodyFromFile()
{
}

void Patch9Sprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		m_bounding->combine(Rect(m_pos, m_symbol->getWidth() * m_xScale * 0.5f,
			m_symbol->getHeight() * m_yScale * 0.5f));
		m_bounding->rotate(m_angle);
	}
}
} // d2d