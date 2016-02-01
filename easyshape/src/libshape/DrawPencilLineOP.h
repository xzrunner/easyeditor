#ifndef _EASYSHAPE_DRAW_PENCIL_LINE_OP_H_
#define _EASYSHAPE_DRAW_PENCIL_LINE_OP_H_

#include "DrawCurveOP.h"

namespace ee { class OneFloatValue; }

namespace eshape
{

class DrawPencilLineOP : public DrawCurveOP
{
public:
	DrawPencilLineOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::OneFloatValue* simplify);

	virtual bool OnMouseLeftUp(int x, int y);

private:
	ee::OneFloatValue* m_simplify;

}; // DrawPencilLineOP

}

#endif // _EASYSHAPE_DRAW_PENCIL_LINE_OP_H_