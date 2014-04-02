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
	m_symbol->retain();
	width = m_symbol->getSize().xLength();
	height = m_symbol->getSize().yLength();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->retain();
	width = m_symbol->getSize().xLength();
	height = m_symbol->getSize().yLength();
	buildBounding();
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->release();
	}
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
	Symbol* scale9 = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && scale9)
	{
		m_symbol->release();
		symbol->retain();

		m_symbol = scale9;
	}
}

void Sprite::loadBodyFromFile()
{
}


}