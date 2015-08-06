#ifndef _LIBSHAPE_DRAW_COMPLEX_POLYGON_OP_H_
#define _LIBSHAPE_DRAW_COMPLEX_POLYGON_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class ComplexPolygonShape;

class DrawComplexPolygonOP : public DrawPolylineOP
{
public:
	DrawComplexPolygonOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		d2d::MultiShapesImpl* shapesImpl);

	virtual bool OnMouseLeftDClick(int x, int y);

private:
	ComplexPolygonShape* CreateComplexPoly(const std::vector<d2d::Vector>& polyline);

protected:
	d2d::MultiShapesImpl* m_shapesImpl;

}; // DrawComplexPolygonOP

}

#endif // _LIBSHAPE_DRAW_COMPLEX_POLYGON_OP_H_