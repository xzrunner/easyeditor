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

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::TextureSprite(sym)
	, ee::Sprite(sym)
{
}

ee::Sprite* Sprite::Create(ee::Symbol* symbol) 
{
	return new Sprite(static_cast<Symbol*>(symbol));
}

}