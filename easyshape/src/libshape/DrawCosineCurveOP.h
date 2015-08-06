#ifndef _LIBSHAPE_DRAW_COSINE_CURVE_OP_H_
#define _LIBSHAPE_DRAW_COSINE_CURVE_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawCosineCurveOP : public DrawPolylineOP
{
public:
	DrawCosineCurveOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		d2d::MultiShapesImpl* shapesImpl);

	virtual bool OnMouseLeftDClick(int x, int y);

private:
	d2d::MultiShapesImpl* m_shapesImpl;

}; // DrawCosineCurveOP

}

#endif // _LIBSHAPE_DRAW_COSINE_CURVE_OP_H_