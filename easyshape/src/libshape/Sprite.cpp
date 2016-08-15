#include "Sprite.h"
#include "Symbol.h"

namespace eshape
{

Sprite::Sprite(Symbol* sym)
	: ee::Sprite(sym)
{
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}