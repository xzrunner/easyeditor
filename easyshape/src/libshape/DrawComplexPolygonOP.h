#ifndef _EASYSHAPE_DRAW_COMPLEX_POLYGON_OP_H_
#define _EASYSHAPE_DRAW_COMPLEX_POLYGON_OP_H_

#include "DrawPolylineOP.h"

namespace eshape
{

class ComplexPolygonShape;

class DrawComplexPolygonOP : public DrawPolylineOP
{
public:
	DrawComplexPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiShapesImpl* shapesImpl);

	virtual bool OnMouseLeftDClick(int x, int y);

private:
	ComplexPolygonShape* CreateComplexPoly(const std::vector<ee::Vector>& polyline);

protected:
	ee::MultiShapesImpl* m_shapesImpl;

}; // DrawComplexPolygonOP

}

#endif // _EASYSHAPE_DRAW_COMPLEX_POLYGON_OP_H_