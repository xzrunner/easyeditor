#ifndef _EASYSHAPE_DRAW_COSINE_CURVE_OP_H_
#define _EASYSHAPE_DRAW_COSINE_CURVE_OP_H_

#include "DrawPolylineOP.h"

namespace eshape
{

class DrawCosineCurveOP : public DrawPolylineOP
{
public:
	DrawCosineCurveOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnMouseLeftDClick(int x, int y);

}; // DrawCosineCurveOP

}

#endif // _EASYSHAPE_DRAW_COSINE_CURVE_OP_H_