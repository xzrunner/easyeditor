#ifndef _SPRITE2_SHAPE_SYMBOL_H_
#define _SPRITE2_SHAPE_SYMBOL_H_

#include "Symbol.h"

#include <stdint.h>

namespace s2
{

class ShapeSymbol : public Symbol
{
public:
	ShapeSymbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

}; // ShapeSymbol

}

#endif // _SPRITE2_SHAPE_SYMBOL_H_