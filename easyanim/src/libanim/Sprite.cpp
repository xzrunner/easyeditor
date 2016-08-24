#include "Sprite.h"
#include "Symbol.h"

#include <ee/std_functor.h>

namespace eanim
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::AnimSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::AnimSprite(sym)
	, ee::Sprite(sym)
{
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}