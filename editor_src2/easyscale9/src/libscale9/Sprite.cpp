#include "Sprite.h"

namespace escale9
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	width = height = 0;
}

Sprite::Sprite(const Sprite& sprite)
	: d2d::ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	width = m_symbol->getSize().xLength();
	height = m_symbol->getSize().yLength();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	width = m_symbol->getSize().xLength();
	height = m_symbol->getSize().yLength();
	buildBounding();
}

Sprite::~Sprite()
{
}

Sprite* Sprite::clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Symbol& Sprite::getSymbol() const
{
	return *m_symbol;
}

void Sprite::setSymbol(d2d::ISymbol* symbol)
{
	Symbol* anim = dynamic_cast<Symbol*>(symbol);
	if (anim) m_symbol = anim;
}

void Sprite::loadBodyFromFile()
{
}


}