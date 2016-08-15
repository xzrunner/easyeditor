#include "Sprite.h"
#include "Symbol.h"

namespace eterrain2d
{

Sprite::Sprite(Symbol* symbol)
	: ee::Sprite(symbol)
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