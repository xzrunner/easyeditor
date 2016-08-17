#include "Sprite.h"
#include "Symbol.h"

namespace etemplate
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
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}