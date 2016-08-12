#ifndef _SPRITE2_PARTICLE2D_SYMBOL_H_
#define _SPRITE2_PARTICLE2D_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Particle2dSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	Particle2dSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const { return sm::rect(0, 0); }

}; // Particle2dSymbol

}

#endif // _SPRITE2_PARTICLE2D_SYMBOL_H_