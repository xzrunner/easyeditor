#ifndef _LIBSHAPE_DRAW_PENCIL_LINE_OP_H_
#define _LIBSHAPE_DRAW_PENCIL_LINE_OP_H_

#include "DrawCurveOP.h"

namespace libshape
{

class DrawPencilLineOP : public DrawCurveOP
{
public:
	DrawPencilLineOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		d2d::MultiShapesImpl* shapesImpl, d2d::OneFloatValue* simplify);

	virtual bool OnMouseLeftUp(int x, int y);

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::OneFloatValue* m_simplify;

}; // DrawPencilLineOP

}

#endif // _LIBSHAPE_DRAW_PENCIL_LINE_OP_H_