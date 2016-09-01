#include "Sprite.h"
#include "Symbol.h"

namespace emask
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::MaskSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::MaskSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::MaskSprite(sym)
	, ee::Sprite(sym)
{
}

bool Sprite::Update(const s2::RenderParams& params) 
{
	Symbol* sym = dynamic_cast<Symbol*>(m_sym);
	const_cast<s2::Sprite*>(sym->GetBase())->Update(params);
	const_cast<s2::Sprite*>(sym->GetMask())->Update(params);
	return true; 
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}