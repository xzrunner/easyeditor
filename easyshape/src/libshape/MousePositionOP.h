#ifndef _EASYSHAPE_MOUSE_POSITION_OP_H_
#define _EASYSHAPE_MOUSE_POSITION_OP_H_

#include "DrawPolylineOP.h"

#include <ee/MousePositionOP.h>

namespace eshape
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

#endif // _EASYSHAPE_MOUSE_POSITION_OP_H_