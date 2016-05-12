#ifndef _SPRITE2_PARTICLE2D_SYMBOL_H_
#define _SPRITE2_PARTICLE2D_SYMBOL_H_

#include "Symbol.h"

#include <stdint.h>

namespace s2
{

class Particle2dSymbol : public Symbol
{
public:
	Particle2dSymbol();

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

}; // Particle2dSymbol

}

#endif // _SPRITE2_PARTICLE2D_SYMBOL_H_