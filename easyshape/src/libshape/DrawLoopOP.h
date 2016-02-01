#ifndef _EASYSHAPE_DRAW_LOOP_OP_H_
#define _EASYSHAPE_DRAW_LOOP_OP_H_

#include "DrawPolylineOP.h"

namespace ee { class MultiShapesImpl; }

namespace eshape
{

class DrawLoopOP : public DrawPolylineOP
{
public:
	DrawLoopOP(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiShapesImpl* shapes_impl);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // DrawLoopOP

}

#endif // _EASYSHAPE_DRAW_LOOP_OP_H_