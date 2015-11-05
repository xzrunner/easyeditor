#ifndef _LIBSHAPE_DRAW_COSINE_CURVE_OP_H_
#define _LIBSHAPE_DRAW_COSINE_CURVE_OP_H_

#include "DrawPolylineOP.h"

namespace libshape
{

class DrawCosineCurveOP : public DrawPolylineOP
{
public:
	DrawCosineCurveOP(wxWindow* wnd, d2d::EditPanelImpl* stage);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // DrawCosineCurveOP

}

#endif // _LIBSHAPE_DRAW_COSINE_CURVE_OP_H_