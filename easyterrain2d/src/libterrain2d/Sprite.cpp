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

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, ee::Sprite(sym)
{
//	rotate(-m_sym->GetOceanAngle());
}

void Sprite::Load(const Json::Value& val, const CU_STR& dir)
{
	ee::Sprite::Load(val);
//	rotate(-m_sym->GetOceanAngle());
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}