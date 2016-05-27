#ifndef _SPRITE2_PARTICLE3D_SYMBOL_H_
#define _SPRITE2_PARTICLE3D_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Particle3dSymbol : public Symbol
{
public:
	Particle3dSymbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr) const;

}; // Particle3dSymbol

}

#endif // _SPRITE2_PARTICLE3D_SYMBOL_H_