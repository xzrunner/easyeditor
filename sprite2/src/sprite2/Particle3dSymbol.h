#ifndef _SPRITE2_PARTICLE3D_SYMBOL_H_
#define _SPRITE2_PARTICLE3D_SYMBOL_H_

#include "Symbol.h"

#include <stdint.h>

namespace s2
{

class Particle3dSymbol : public Symbol
{
public:
	Particle3dSymbol();

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

}; // Particle3dSymbol

}

#endif // _SPRITE2_PARTICLE3D_SYMBOL_H_