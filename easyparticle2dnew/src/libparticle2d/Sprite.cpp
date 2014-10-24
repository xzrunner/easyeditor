#include "Sprite.h"

namespace eparticle2d
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->retain();
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
	Symbol* particle3d = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && particle3d)
	{
		m_symbol->release();
		symbol->retain();

		m_symbol = particle3d;
	}
}

void Sprite::loadBodyFromFile()
{
}

}