#include "Sprite.h"

namespace eejoy2d
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	if (m_symbol) {
		m_symbol->Retain();
	}
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	if (m_symbol) {
		m_symbol->Retain();
	}
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
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
	Symbol* ejoy2d = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && ejoy2d)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = ejoy2d;
	}
}

}