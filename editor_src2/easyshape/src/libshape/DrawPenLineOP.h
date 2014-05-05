#ifndef _LIBSHAPE_DRAW_PEN_LINE_OP_H_
#define _LIBSHAPE_DRAW_PEN_LINE_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawPenLineOP : public DrawPolylineOP
{
public:
	DrawPenLineOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

	virtual bool onMouseLeftDClick(int x, int y);

private:
	d2d::MultiShapesImpl* m_shapesImpl;

}; // DrawPenLineOP

}

#endif // _LIBSHAPE_DRAW_PEN_LINE_OP_H_