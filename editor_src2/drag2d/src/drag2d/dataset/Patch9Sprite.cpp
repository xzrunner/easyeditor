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
	width = m_symbol->getSize().xLength();
	height = m_symbol->getSize().yLength();
}

Patch9Sprite::Patch9Sprite(Patch9Symbol* symbol)
	: m_symbol(symbol)
{
	width = m_symbol->getSize().xLength();
	height = m_symbol->getSize().yLength();
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

} // d2d