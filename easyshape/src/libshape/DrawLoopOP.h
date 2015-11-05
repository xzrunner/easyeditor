#ifndef _LIBSHAPE_DRAW_LOOP_OP_H_
#define _LIBSHAPE_DRAW_LOOP_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawLoopOP : public DrawPolylineOP
{
public:
	DrawLoopOP(wxWindow* wnd, d2d::EditPanelImpl* stage,
		d2d::MultiShapesImpl* shapesImpl);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // DrawLoopOP

}

#endif // _LIBSHAPE_DRAW_LOOP_OP_H_