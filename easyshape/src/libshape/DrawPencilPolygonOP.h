#ifndef _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_
#define _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_

#include "DrawCurveOP.h"

namespace libshape
{

class DrawPencilPolygonOP : public DrawCurveOP
{
public:
	DrawPencilPolygonOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::OneFloatValue* simplify);

	virtual bool onMouseLeftUp(int x, int y);

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::OneFloatValue* m_simplify;

}; // DrawPencilPolygonOP

}

#endif // _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_