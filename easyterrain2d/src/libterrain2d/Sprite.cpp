#include "Sprite.h"

namespace eterrain2d
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& s)
	: ee::Sprite(s)
{
	m_symbol = s.m_symbol->Clone();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
//	rotate(-m_symbol->GetOceanAngle());
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