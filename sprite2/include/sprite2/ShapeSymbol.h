#ifndef _SPRITE2_SHAPE_SYMBOL_H_
#define _SPRITE2_SHAPE_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class Shape;

class ShapeSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ShapeSymbol();
	virtual ~ShapeSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	void SetShape(Shape* shape);
	const Shape* GetShape() const { return m_shape; }
	
protected:
	Shape* m_shape;

}; // ShapeSymbol

}

#endif // _SPRITE2_SHAPE_SYMBOL_H_