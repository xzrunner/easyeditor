#include "Sprite.h"

namespace eterrain2d
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& s)
	: d2d::Sprite(s)
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

void Sprite::SetSymbol(d2d::Symbol* symbol)
{
	d2d::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	d2d::Sprite::Load(val);
//	rotate(-m_symbol->GetOceanAngle());
}

}