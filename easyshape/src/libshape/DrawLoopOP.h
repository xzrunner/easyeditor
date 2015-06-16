#ifndef _LIBSHAPE_DRAW_LOOP_OP_H_
#define _LIBSHAPE_DRAW_LOOP_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawLoopOP : public DrawPolylineOP
{
public:
	DrawLoopOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

	virtual bool OnMouseLeftDClick(int x, int y);

private:
	d2d::MultiShapesImpl* m_shapesImpl;

}; // DrawLoopOP

}

#endif // _LIBSHAPE_DRAW_LOOP_OP_H_