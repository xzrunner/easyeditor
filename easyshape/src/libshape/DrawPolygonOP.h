#ifndef _EASYSHAPE_DRAW_POLYGON_OP_H_
#define _EASYSHAPE_DRAW_POLYGON_OP_H_

#include "DrawPolylineOP.h"

namespace eshape
{

class DrawPolygonOP : public DrawPolylineOP
{
public:
	DrawPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiShapesImpl* shapesImpl);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // DrawPolygonOP

}

#endif // _EASYSHAPE_DRAW_POLYGON_OP_H_