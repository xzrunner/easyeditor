#ifndef _EASYSHAPE_DRAW_POLYGON_OP_H_
#define _EASYSHAPE_DRAW_POLYGON_OP_H_

#include "DrawPolylineOP.h"

namespace ee { class MultiShapesImpl; }

namespace eshape
{

class DrawPolygonOP : public DrawPolylineOP
{
public:
	DrawPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiShapesImpl* shapes_impl);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // DrawPolygonOP

}

#endif // _EASYSHAPE_DRAW_POLYGON_OP_H_