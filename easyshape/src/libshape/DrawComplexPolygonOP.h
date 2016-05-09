#ifndef _EASYSHAPE_DRAW_COMPLEX_POLYGON_OP_H_
#define _EASYSHAPE_DRAW_COMPLEX_POLYGON_OP_H_

#include "DrawPolylineOP.h"

namespace ee { class MultiShapesImpl; }

namespace eshape
{

class ComplexPolygonShape;

class DrawComplexPolygonOP : public DrawPolylineOP
{
public:
	DrawComplexPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MultiShapesImpl* shapes_impl);

	virtual bool OnMouseLeftDClick(int x, int y);

private:
	ComplexPolygonShape* CreateComplexPoly(const std::vector<sm::vec2>& polyline);

protected:
	ee::MultiShapesImpl* m_shapes_impl;

}; // DrawComplexPolygonOP

}

#endif // _EASYSHAPE_DRAW_COMPLEX_POLYGON_OP_H_