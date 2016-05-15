#include "Sprite.h"

#include <sprite2/DummySprite.h>

namespace eterrain2d
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_core = new s2::DummySprite(this);
}

Sprite::Sprite(const Sprite& s)
	: ee::Sprite(s)
{
	m_core = new s2::DummySprite(this);

	m_symbol = s.m_symbol->Clone();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::DummySprite(this);

//	rotate(-m_symbol->GetOceanAngle());
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
	return new Sprite(*this);
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	ee::Sprite::Load(val);
//	rotate(-m_symbol->GetOceanAngle());
}

}