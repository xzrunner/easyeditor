#include "Sprite.h"
#include "Symbol.h"

namespace libanis
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

ee::Sprite* Sprite::Create(ee::Symbol* symbol) 
{
	return new Sprite(static_cast<Symbol*>(symbol));
}

}