#include "Sprite.h"
#include "Symbol.h"
#include "PolygonShape.h"

#include <ee/SpriteFactory.h>

#include <sprite2/DummySprite.h>

namespace eshape
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_core = new s2::DummySprite;
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_core = new s2::DummySprite(*static_cast<s2::DummySprite*>(sprite.m_core));

	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::DummySprite;

	m_symbol->Retain();
	BuildBounding();
}

Sprite::~Sprite()
{
	delete m_core; m_core = NULL;

	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	ee::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

}