#include "Sprite.h"
#include "Symbol.h"

namespace eshadow
{

Sprite::Sprite(Symbol* symbol)
	: ee::Sprite(symbol)
{
}

ee::Sprite* Sprite::Create(ee::Symbol* symbol) 
{
	return new Sprite(static_cast<Symbol*>(symbol));
}

}