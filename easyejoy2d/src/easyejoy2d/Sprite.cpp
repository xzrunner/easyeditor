#include "Sprite.h"
#include "Symbol.h"

namespace eejoy2d
{

Sprite::Sprite(Symbol* symbol)
	: ee::Sprite(symbol)
{
}

ee::Sprite* Create(ee::Symbol* symbol) 
{
	return new Sprite(static_cast<Symbol*>(symbol));
}

}