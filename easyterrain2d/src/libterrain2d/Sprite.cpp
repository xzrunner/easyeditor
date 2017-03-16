#include "Sprite.h"
#include "Symbol.h"

namespace eterrain2d
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, ee::Sprite(sym)
{
//	rotate(-m_sym->GetOceanAngle());
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);
//	rotate(-m_sym->GetOceanAngle());
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}