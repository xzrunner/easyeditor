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
	buildBounding();

	m_symbol->ResetPS();
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
	Symbol* particle3d = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && particle3d)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = particle3d;
	}
}

}