#ifndef _EASYEDITOR_MOUSE_POSITION_OP_H_
#define _EASYEDITOR_MOUSE_POSITION_OP_H_

#include "ZoomViewOP.h"

namespace ee
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

#endif // _EASYEDITOR_MOUSE_POSITION_OP_H_