#ifndef _LIBSHAPE_DRAW_COMPLEX_POLYGON_OP_H_
#define _LIBSHAPE_DRAW_COMPLEX_POLYGON_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class ComplexPolygonShape;

class DrawComplexPolygonOP : public DrawPolylineOP
{
public:
	DrawComplexPolygonOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

	virtual bool onMouseLeftDClick(int x, int y);

private:
	ComplexPolygonShape* CreateComplexPoly(const std::vector<d2d::Vector>& polyline);

protected:
	d2d::MultiShapesImpl* m_shapesImpl;

}; // DrawComplexPolygonOP

}

#endif // _LIBSHAPE_DRAW_COMPLEX_POLYGON_OP_H_