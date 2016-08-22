#include "Sprite.h"
#include "Symbol.h"

#include <sprite2/RenderParams.h>

namespace ecomplex
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::ComplexSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::ComplexSprite(sym)
	, ee::Sprite(sym)
{
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}