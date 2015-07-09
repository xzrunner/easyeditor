#include "Sprite.h"
#include "Symbol.h"

namespace eshadow
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
	BuildBounding();
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

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	Symbol* shadow = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && shadow)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = shadow;
	}
}

}