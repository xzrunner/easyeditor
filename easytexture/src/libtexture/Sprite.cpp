#include "Sprite.h"
#include "Symbol.h"

namespace etexture
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::TextureSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::TextureSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::TextureSprite(sym)
	, ee::Sprite(sym)
{
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}