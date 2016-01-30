#include "Sprite.h"

namespace ep3dinv
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();
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
	d2d::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::Symbol* symbol)
{
	d2d::Sprite::SetSymbol(&m_symbol, symbol);
}

}