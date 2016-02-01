#ifndef _EASYSHAPE_DRAW_PEN_LINE_OP_H_
#define _EASYSHAPE_DRAW_PEN_LINE_OP_H_

#include "DrawPolylineOP.h"

namespace ee { class MultiShapesImpl; }

namespace eshape
{

class DrawPenLineOP : public DrawPolylineOP
{
public:
	DrawPenLineOP(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiShapesImpl* shapes_impl);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // DrawPenLineOP

}

#endif // _EASYSHAPE_DRAW_PEN_LINE_OP_H_