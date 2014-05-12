#ifndef _LIBSHAPE_DRAW_POLYGON_EDGE_OP_H_
#define _LIBSHAPE_DRAW_POLYGON_EDGE_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawPolygonEdgeOP : public DrawPolylineOP
{
public:
	DrawPolygonEdgeOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

	virtual bool onMouseLeftDClick(int x, int y);

private:
	d2d::MultiShapesImpl* m_shapesImpl;

}; // DrawPolygonEdgeOP

}

#endif // _LIBSHAPE_DRAW_POLYGON_EDGE_OP_H_