#ifndef _DRAG2D_MOUSE_POSITION_OP_H_
#define _DRAG2D_MOUSE_POSITION_OP_H_

#include "ZoomViewOP.h"

namespace d2d
{

class MousePositionCMPT;

class MousePositionOP : public ZoomViewOP
{
public:
	MousePositionOP(wxWindow* wnd, EditPanelImpl* stage, 
		MousePositionCMPT* cmpt);

	virtual bool OnMouseMove(int x, int y);

private:
	MousePositionCMPT* m_cmpt;

}; // MousePositionOP

}

#endif // _DRAG2D_MOUSE_POSITION_OP_H_