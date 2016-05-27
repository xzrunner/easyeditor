#ifndef _SPRITE2_SHAPE_SYMBOL_H_
#define _SPRITE2_SHAPE_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Shape;

class ShapeSymbol : public Symbol
{
public:
	ShapeSymbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr) const;

	void SetShape(Shape* shape);

private:
	Shape* m_shape;

}; // ShapeSymbol

}

#endif // _SPRITE2_SHAPE_SYMBOL_H_