#include "Sprite.h"
#include "Symbol.h"

namespace eterrain2d
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::DummySprite(spr)
	, ee::Sprite(spr)
{
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::DummySprite(sym)
	, ee::Sprite(sym)
{
//	rotate(-m_symbol->GetOceanAngle());
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);
//	rotate(-m_symbol->GetOceanAngle());
}

ee::Sprite* Sprite::Create(ee::Symbol* symbol) 
{
	return new Sprite(static_cast<Symbol*>(symbol));
}

}