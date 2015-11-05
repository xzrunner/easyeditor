#ifndef _LIBSHAPE_DRAW_PEN_LINE_OP_H_
#define _LIBSHAPE_DRAW_PEN_LINE_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawPenLineOP : public DrawPolylineOP
{
public:
	DrawPenLineOP(wxWindow* wnd, d2d::EditPanelImpl* stage,
		d2d::MultiShapesImpl* shapesImpl);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // DrawPenLineOP

}

#endif // _LIBSHAPE_DRAW_PEN_LINE_OP_H_