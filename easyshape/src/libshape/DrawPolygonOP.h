#ifndef _LIBSHAPE_DRAW_POLYGON_OP_H_
#define _LIBSHAPE_DRAW_POLYGON_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawPolygonOP : public DrawPolylineOP
{
public:
	DrawPolygonOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		d2d::MultiShapesImpl* shapesImpl);

	virtual bool OnMouseLeftDClick(int x, int y);

protected:
	d2d::MultiShapesImpl* m_shapesImpl;

}; // DrawPolygonOP

}

#endif // _LIBSHAPE_DRAW_POLYGON_OP_H_