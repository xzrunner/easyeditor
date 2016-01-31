#ifndef _LIBSHAPE_MOUSE_POSITION_OP_H_
#define _LIBSHAPE_MOUSE_POSITION_OP_H_



#include "DrawPolylineOP.h"

namespace libshape
{

class MousePositionOP : public ee::MousePositionOP 
{
public:
	MousePositionOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		ee::MousePositionCMPT* cmpt);
	virtual ~MousePositionOP();

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

private:
	DrawPolylineOP* m_draw_impl;

}; // MousePositionOP

}

#endif // _LIBSHAPE_MOUSE_POSITION_OP_H_